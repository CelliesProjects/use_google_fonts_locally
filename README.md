# How to host Google fonts through ESPAsyncWebServer

In this example we are going to serve the Google Roboto font from your esp32 instead of through the Google API. This means downloading the font files, renaming and adjusting them, converting to C-style header files and use them with ESPAsynWebServer.

1. Locate the font on [https://fonts.google.com/](https://fonts.google.com/). 

2. You will need to download the font through the google font API which is `https://fonts.googleapis.com/css2`.   
See [https://developers.google.com/fonts/docs/css2](). No account is needed to download the font files.

3. Open the link [https://fonts.googleapis.com/css2?family=Roboto](https://fonts.googleapis.com/css2?family=Roboto) in your browser and locate your local font variants. 
In this case `latin` and `latin-ext`. (Western alphabet and western accented letters) 

4. Find the desired font variants and download the files linked to in the line `src: url()` to the folder where your sketch is. These are for Roboto `https://fonts.gstatic.com/s/roboto/v30/KFOmCnqEu92Fr1Mu7GxKOzY.woff2` for latin-ext and `https://fonts.gstatic.com/s/roboto/v30/KFOmCnqEu92Fr1Mu4mxK.woff2` for latin.

5. Rename the files to some easier names to remember, I used `Roboto-latin.woff2` and `Roboto-latin-ext.woff2`.

6. Copy the complete css `@font-face{}` section containg the desired variants to the html file hosted on the esp32. Change the `src: url` lines like below  
```/* latin-ext */
@font-face {
  font-family: 'Roboto';
  font-style: normal;
  font-weight: 400;
  src: url(Roboto-latin-ext.woff2) format('woff2');
  unicode-range: U+0100-024F, U+0259, U+1E00-1EFF, U+2020, U+20A0-20AB, U+20AD-20CF, U+2113, U+2C60-2C7F, U+A720-A7FF;
}
/* latin */
@font-face {
  font-family: 'Roboto';
  font-style: normal;
  font-weight: 400;
  src: url(/Roboto-latin.woff2) format('woff2');
  unicode-range: U+0000-00FF, U+0131, U+0152-0153, U+02BB-02BC, U+02C6, U+02DA, U+02DC, U+2000-206F, U+2074, U+20AC, U+2122, U+2191, U+2193, U+2212, U+2215, U+FEFF, U+FFFD;
}
```

7. To use the files in your project convert the font files to C-style header files with:  
`xxd -i inputfile > outputfile`.  
This would be `xxd -i Roboto-latin.woff2 > Roboto-latin.h` for the latin file and `xxd -i Roboto-latin-ext.woff2 > Roboto-latin-ext.h` for both files.

8. In the generated files, change the first line to `const unsigned char Roboto_latin_woff2[] = {` and the last line to `const unsigned int Roboto_latin_woff2_len = 15744;`   
If you don't declare the array as a const, it will end up in RAM memory instead of in FLASH memory.

9. Add `#include #include "Roboto-latin.h"` and `#include "Roboto-latin-ext.h"` in your sketch.

10. Set up ASyncWebServer to use the `Content-Type` header `application/x-font-woff2` when serving these files.
