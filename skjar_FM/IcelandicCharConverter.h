#ifndef ICELANDIC_CHAR_CONVERTER_H
#define ICELANDIC_CHAR_CONVERTER_H

#include <Arduino.h>

// Function to convert Icelandic characters to simpler equivalents
String convertIcelandicChars(String input) {
  // Lowercase conversions
  input.replace("ð", "d");
  input.replace("á", "a");
  input.replace("é", "e");
  input.replace("í", "i");
  input.replace("ó", "o");
  input.replace("ú", "u");
  input.replace("ý", "y");
  input.replace("þ", "th");
  input.replace("æ", "ae");
  input.replace("ö", "o");

  // Uppercase conversions
  input.replace("Ð", "D");
  input.replace("Á", "A");
  input.replace("É", "E");
  input.replace("Í", "I");
  input.replace("Ó", "O");
  input.replace("Ú", "U");
  input.replace("Ý", "Y");
  input.replace("Þ", "TH");
  input.replace("Æ", "AE");
  input.replace("Ö", "O");

  return input;
}

#endif // ICELANDIC_CHAR_CONVERTER_H