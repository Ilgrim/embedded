pcb_height = 58.420;
pcb_length = 45.720 + 1.27;
pcb_thickness = 1.6 + 0.4;

stand_width = pcb_length + 4;
stand_length = 30.0;
stand_height = 32.0;

usb_height = 2.80;
usb_width = 5.10 + 0.15; // Includes tolerance
usb_length = 7.90 + 0.15; // includes tolerance

usb_cutout_length = usb_length + 5;
usb_cutout_thickness = usb_width + 5;
usb_cutout_depth = stand_height + 5;

pcb_cutout_length = pcb_length;
pcb_cutout_thickness = pcb_thickness;
pcb_cutout_depth = 5.0;

cable_cutout_diameter = 5 + 1;

union() {
    stand();
}

module stand() {
    difference() {
        cube([stand_width,
              stand_length,
              stand_height],
             center = true);
        cutout();
        cable_cutout();
    }
}


module cutout() {
    pcb_cutout_x = -(pcb_cutout_length / 2);
    pcb_cutout_y = -(pcb_cutout_thickness / 2);
    pcb_cutout_z = (stand_height / 2) - pcb_cutout_depth + 1;

    usb_cutout_x = -(usb_cutout_length / 2);
    usb_cutout_y = -(usb_cutout_thickness - 2.5);
    usb_cutout_z = (stand_height / 2) - usb_cutout_depth + 1;

    union() {
        translate([pcb_cutout_x, pcb_cutout_y, pcb_cutout_z])
            cube([pcb_cutout_length,
                  pcb_cutout_thickness,
                  pcb_cutout_depth],
                 cunter = true);
        translate([usb_cutout_x, usb_cutout_y, usb_cutout_z])
            cube([usb_cutout_length,
                  usb_cutout_thickness,
                  usb_cutout_depth],
                 cunter = true);
    }
}

module cable_cutout() {
    cable_cutout_x = 0 - (cable_cutout_diameter / 2);
    cable_cutout_y = 0;
    cable_cutout_z = -(stand_height / 2) - 1;

    translate([cable_cutout_x, cable_cutout_y, cable_cutout_z])
        cube([cable_cutout_diameter,
              20,
              cable_cutout_diameter]);
}
