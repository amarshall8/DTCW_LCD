void recvWithStartEndMarkers() {
    boolean newData = false;
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    memset(commandFromTCU,0,sizeof(commandFromTCU));
    integerFromTCU = 0;
    floatFromTCU = 0.0;

    while (tempBoard.available() > 0 && newData == false) {
        rc = tempBoard.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        //showParsedData();
        newData = false;
    }
}



//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(commandFromTCU, strtokIndx); // copy it to commandFromLCD
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    integerFromTCU = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    floatFromTCU = atof(strtokIndx);     // convert this part to a float

}

//============

void showParsedData() {
    Serial.print("Message ");
    Serial.println(commandFromTCU);
    Serial.print("Integer ");
    Serial.println(integerFromTCU);
    Serial.print("Float ");
    Serial.println(floatFromTCU);
}