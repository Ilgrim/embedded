#include "osc.h"

#include <cstdint>

namespace nome {

int16_t osc_wavetable[256];

static const int16_t sin_wavetable[] = {
  -32512, -32502, -32473, -32423,
  -32356, -32265, -32160, -32031,
  -31885, -31719, -31533, -31331,
  -31106, -30864, -30605, -30324,
  -30028, -29712, -29379, -29026,
  -28658, -28272, -27868, -27449,
  -27011, -26558, -26089, -25604,
  -25103, -24588, -24056, -23512,
  -22953, -22378, -21793, -21191,
  -20579, -19954, -19316, -18667,
  -18006, -17334, -16654, -15960,
  -15259, -14548, -13828, -13100,
  -12363, -11620, -10868, -10112,
   -9347,  -8578,  -7805,  -7023,
   -6241,  -5453,  -4662,  -3868,
   -3073,  -2274,  -1474,   -674,
     126,    929,   1729,   2527,
    3326,   4123,   4916,   5707,
    6495,   7278,   8057,   8833,
    9601,  10366,  11122,  11874,
   12618,  13353,  14082,  14802,
   15512,  16216,  16906,  17589,
   18260,  18922,  19569,  20207,
   20834,  21446,  22045,  22634,
   23206,  23765,  24311,  24842,
   25357,  25858,  26343,  26812,
   27266,  27701,  28123,  28526,
   28912,  29281,  29632,  29966,
   30281,  30579,  30859,  31118,
   31361,  31583,  31788,  31973,
   32139,  32286,  32412,  32521,
   32608,  32679,  32725,  32757,
   32766,  32757,  32725,  32679,
   32608,  32521,  32412,  32286,
   32139,  31973,  31788,  31583,
   31361,  31118,  30859,  30579,
   30281,  29966,  29632,  29281,
   28912,  28526,  28123,  27701,
   27266,  26812,  26343,  25858,
   25357,  24842,  24311,  23765,
   23206,  22634,  22045,  21446,
   20834,  20207,  19569,  18922,
   18260,  17589,  16906,  16216,
   15512,  14802,  14082,  13353,
   12618,  11874,  11122,  10366,
    9601,   8833,   8057,   7278,
    6495,   5707,   4916,   4123,
    3326,   2527,   1729,    929,
     126,   -674,  -1474,  -2274,
   -3073,  -3868,  -4662,  -5453,
   -6241,  -7023,  -7805,  -8578,
   -9347, -10112, -10868, -11620,
  -12363, -13100, -13828, -14548,
  -15259, -15960, -16654, -17334,
  -18006, -18667, -19316, -19954,
  -20579, -21191, -21793, -22378,
  -22953, -23512, -24056, -24588,
  -25103, -25604, -26089, -26558,
  -27011, -27449, -27868, -28272,
  -28658, -29026, -29379, -29712,
  -30028, -30324, -30605, -30864,
  -31106, -31331, -31533, -31719,
  -31885, -32031, -32160, -32265,
  -32356, -32423, -32473, -32502,
  -32512,
};

Osc::Osc(OscShape shape,
         const uint32_t sample_rate,
         const uint16_t freq_hz)
    : shape_(shape),
      sample_rate_(sample_rate),
      freq_hz_(freq_hz),
      phase_(0) {
    assign_lookup_table();
    compute_phase_increment();
    compute_next_value();
}

void Osc::Tick() {
    phase_++;
    value_ = compute_next_value();
}

int16_t Osc::Value() {
    return value_;
}

void Osc::set_freq(uint16_t freq_hz) {
    freq_hz_ = freq_hz;
    phase_ = 0;
    compute_phase_increment();
    value_ = compute_next_value();
}

void Osc::set_shape(OscShape shape) {
    shape_ = shape;
    phase_ = 0;
    assign_lookup_table();
    compute_phase_increment();
    value_ = compute_next_value();
}

void Osc::compute_phase_increment() {
    phase_increment_ = table_size_ * ((freq_hz_ << 16) / sample_rate_);
}

int16_t Osc::compute_next_value() {
    uint32_t index = ((phase_ * phase_increment_) >> 16) % table_size_;
    if (index == 0) {
        // Reset the phase to avoid overflows
        phase_ = 0;
    }

    return table_[index];
}

void Osc::assign_lookup_table() {
    switch (shape_) {
        case OscShape::SIN:
            table_ = (int16_t *)sin_wavetable;
            table_size_ = sizeof(sin_wavetable) / sizeof(int16_t);
            break;
        case OscShape::SQUARE:
            table_ = osc_wavetable;
            table_size_ = sizeof(osc_wavetable) / sizeof(int16_t);
            generate_square_table();
            break;
        case OscShape::TRIANGLE:
            table_ = osc_wavetable;
            table_size_ = sizeof(osc_wavetable) / sizeof(int16_t);
            generate_triangle_table();
            break;
        default:
            table_ = (int16_t *)sin_wavetable;
            table_size_ = sizeof(sin_wavetable) / sizeof(int16_t);
            break;
    }
}

void Osc::generate_square_table() {
    int16_t sample;
    for (size_t i = 0; i < table_size_; i++) {
        if (i < (table_size_ / 2)) {
            sample = INT16_MIN;
        } else {
            sample = INT16_MAX;
        }
        table_[i] = sample;
    }
}

void Osc::generate_triangle_table() {
    const int16_t step = UINT16_MAX / table_size_;
    const int16_t start = INT16_MIN;
    
    int16_t sample = start;
    for (size_t i = 0; i < table_size_; i++) {
        table_[i] = sample;
        if (i < (table_size_ / 2)) {
            sample += step;
        } else {
            sample -= step;
        }
    }
}

}
