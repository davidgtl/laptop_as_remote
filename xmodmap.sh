xmodmap -pke > ~/.xmodmap_original
xmodmap ~/.xmodmap_original
clear lock
clear mod2
keycode  38 = Caps_Lock
keycode  77 = Num_Lock
add lock = Caps_Lock
add mod2 = Num_Lock

