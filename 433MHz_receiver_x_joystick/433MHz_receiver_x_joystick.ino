// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dEpendant SPI Library
#include <SPI.h>
// Create Amplitude Shift Keying Obj ect

RH_ASK rf_driver;

void setup() {
  // put your setup code here, to run once:
  rf_driver.init();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t buf[13];
  uint8_t buflen = sizeof(buf);

  if(rf_driver.recv(buf, &buflen)){

    // Convert received data to string
    String str_out = String((char*)buf);

    // Serial.print("Message Received: ");
    // Serial.println(str_out);

    int numElements = countElements(str_out, ',');
    String* result = splitByComma(str_out, numElements);

    int xVal = parseIntSafe(result[0]);
    int yVal = parseIntSafe(result[1]);

    Serial.print("X = ");
    Serial.print(xVal);

    Serial.print("     Y = ");
    Serial.print(yVal);
    Serial.println(" ");

    delete[] result;
  }

  // Clean up allocated memory
}

// Function to count the number of elements separated by a delimiter
int countElements(String data, char delimiter) {
  int count = 1; // there's at least one element
  for (int i = 0; i < data.length(); i++) {
    if (data.charAt(i) == delimiter) {
      count++;
    }
  }
  return count;
}

// Function to filter and keep only numbers and signs
String filterValidChars(String data) {
  String filtered = "";
  for (int i = 0; i < data.length(); i++) {
    char c = data.charAt(i);
    if (isDigit(c) || c == '-' || c == '+') {
      filtered += c;
    }
  }
  return filtered;
}

// Function to split a string by commas
String* splitByComma(String data, int numElements) {
  String* result = new String[numElements];
  int index = 0;

  while (data.length() > 0) {
    int commaIndex = data.indexOf(',');
    String element;
    if (commaIndex == -1) {
      element = data;
      data = "";
    } else {
      element = data.substring(0, commaIndex);
      data = data.substring(commaIndex + 1);
    }
    result[index++] = filterValidChars(element);
  }

  return result;
}

int parseIntSafe(String str) {
  // Remove leading zeros
  str = removeLeadingZeros(str);

  // Check if the string is empty after removing zeros
  if (str.length() == 0) {
    return 0;
  }

  // Iterate through each character to validate it
  for (int i = 0; i < str.length(); i++) {
    if (i == 0 && (str.charAt(i) == '-' || str.charAt(i) == '+')) {
      // Allow leading '+' or '-' sign
      continue;
    }
    if (!isDigit(str.charAt(i))) {
      // If any character is not a digit, return 0
      return 0;
    }
  }

  // Convert the valid string to an integer
  return str.toInt();
}

String removeLeadingZeros(String str) {
  int startIndex = 0;
  int length = str.length();

  // If the string starts with a sign, skip it
  if (length > 0 && (str.charAt(0) == '-' || str.charAt(0) == '+')) {
    startIndex = 1;
  }

  // Find the index of the first non-zero character
  while (startIndex < length && str.charAt(startIndex) == '0') {
    startIndex++;
  }

  // If all characters are zeros, return a single zero
  if (startIndex == length) {
    return "0";
  }

  // Return the string without leading zeros
  if (startIndex == 1 && (str.charAt(0) == '-' || str.charAt(0) == '+')) {
    return str.charAt(0) + str.substring(startIndex);
  }
  
  return str.substring(startIndex);
}