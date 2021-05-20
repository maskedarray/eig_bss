#include <cmdlib-master.h>

#define PAIRING_ID "abcdef"



/**
 * @brief This is a generalized function used to parse the message (after the
 * ID) to return a value at a certain position (or key) in the message.
 *
 * @param message the received string
 * @param key a key for the required information. It must not exceed the total
 * number of entries in the message 
 * @return String of the parsed value at the given key. In the case an invalid
 * key is used an empty string will be returned 
 */
String parse_by_key(String message, int key)
{
    char temp = '\0';
    int index = 0;
    int comma_count = 0;
    int counter = 0;
    String key_value = "";

    while(message[index] != '>')
    {
        if(message[index] == ',')
        {
            comma_count++;
        }
        index++;
    }
    
    index = 0;
    if(key > comma_count)
    {
        log_e("key exceeds number of entries ");
        return "";
    }
    
    while(counter != key && counter <= comma_count && message[index] != '>')
    {
        if(message[index] == ',')
        {
            counter++;
        }
        index++;
    }

    while(message[index] != '>' && message[index] != ',')
    {
        temp = message[index];
        key_value += temp;
        index++;
    }
    return key_value;

};


/**
 * @brief This command is used to create a new connection for wifi and store it
 * in the list of APs.
 *
 * @param message the received string
 * @return true if connection instruction is sent
 * @return false otherwise
 */
bool command_3_newConn(String message)
{
    bool ret = false;
    String tempssid = parse_by_key(message, 1);
    String temppass = parse_by_key(message, 2);
    ret = wf.create_new_connection(tempssid.c_str(),temppass.c_str());
    return ret;
};

/**
 * @brief this command tells the slave to check the wifi connection and send the
 * status back to the master
 *
 * @return true if command has been sent
 * @return false if command could not be sent
 */
bool command_7_checkWifi()
{
    bool ret = WiFi.isConnected();
    log_i("the connection status is: %d\n\r", ret);
    return ret;
}

/**
 * @brief This command returns the unix time
 * 
 * @return true if command returns successful
 * @return false otherwise
 */
bool command_8_getTime()
{
    String ret = unixTime();
    log_d("The returned time is: %s", ret);
    bt.send(ret);
    return true;
}

/**
 * @brief This command implements battery ejection upon bluetooth command
 * 
 * @param message the received string containing the battery to be ejected
 * @return true if eject is successful
 * @return false otherwise
 */
bool command_9_ejectBattery(String message){
    int slot_num = parse_by_key(message,1).toInt();
    log_i("ejected battery %d",slot_num);
    return true;
    //eject battery by handling the bss through CAN
}

/**
 * @brief This is the main wrapper function that is called in a loop and checks
 * for commands on bluetooth. It also checks if authorization has been granted
 * for different functions and resets authorization after every command call.
 *
 * @return true if command returns true
 * @return false otherwise
 */
bool command_bt()
{
    String message = "";
    message = bt.check_bluetooth();
    
    if(message.length() > 0)
    {
        log_i("message received: %s", message.c_str());
        int ID = (10 * ((uint8_t)message[1] - 48)) + ((uint8_t)message[2] - 48);
        log_d("the the ID sent is: %d", ID);
        if(ID > 0 && ID < 100)
        {
            switch(ID)
            {
                case 3: // connect to new credentials
                    return command_3_newConn(message);
                case 7: // check wifi
                    return command_7_checkWifi();
                case 8: // check time
                    return command_8_getTime();
                case 9: // eject specified battery
                    return command_9_ejectBattery(message);
                default:
                    log_e("invalid ID ");
                    return false;
            }
        }
        else
        {
            log_e("entered invalid ID ");
            return false;
        }

    }
    else
    {
        return false;
    }
}