#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPTION_LIST_H
#ವ್ಯಾಖ್ಯಾನಿಸು OPTION_LIST_H
#ಅನ್ತರ್ಭಾವ "list.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ{
    ಅಕ್ಷರ *ಕೀಲಿ;
    ಅಕ್ಷರ *val;
    ಪೂರ್ಣಾಂಕ ಬಳಸಿದ;
} kvp;


ಪೂರ್ಣಾಂಕ read_option(ಅಕ್ಷರ *s, list *options);
ಶೂನ್ಯ option_ಸೇರಿಸು(list *l, ಅಕ್ಷರ *ಕೀಲಿ, ಅಕ್ಷರ *val);
ಅಕ್ಷರ *option_ಕಂಡುಹಿಡಿ(list *l, ಅಕ್ಷರ *ಕೀಲಿ);
ದಶಮಾಂಶ option_ಕಂಡುಹಿಡಿ_float(list *l, ಅಕ್ಷರ *ಕೀಲಿ, ದಶಮಾಂಶ def);
ದಶಮಾಂಶ option_ಕಂಡುಹಿಡಿ_float_quiet(list *l, ಅಕ್ಷರ *ಕೀಲಿ, ದಶಮಾಂಶ def);
ಶೂನ್ಯ option_ಬಳಸದ(list *l);

#ಆದರೆ_ಅಂತ್ಯ
