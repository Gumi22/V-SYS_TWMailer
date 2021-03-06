//
// Created by alexander on 12.10.17.
//


#include "SendMessage.h"



SendMessage::SendMessage(const char *directory, User* user) : ServerOperation(directory, user) {
    index = 1;
    statusMessage = "Receiver: ";
}

string SendMessage::execute() {
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    );
    boost::uuids::random_generator gen;
    /// save receiver with \n on the end of the string
    string receiver_print = receiver;
    ///delete \n from the end of the string for routing in directories
    receiver.pop_back();
    ///build correct path into directory of the receiver
    string path_of_Directory = string(MESSAGEDIR) + '/' + receiver;
    ///build a unique path to the mail txt file
    boost::uuids::uuid new_Filename = gen();
    std::string filename = to_string(ms.count()) + boost::lexical_cast<std::string> (new_Filename);
    string path_to_file = string(MESSAGEDIR) + '/' + receiver + '/' +  filename +  ".txt";
    string path_to_attachment_dir = string(MESSAGEDIR) + '/' + receiver + "/attachments"; //ToDo: make that a define
    string path_to_attachment = path_to_attachment_dir + '/' + filename;
    ///convert all strings into char * for functions opendir, mkdir, fprintf
    const char * path = path_of_Directory.c_str();
    const char * path_to_File = path_to_file.c_str();
    ///check if the directory is available, if not - create new
    DIR* test = opendir(path);
    if(test == nullptr){
        mkdir(path, 0777);
    }
    else{
        closedir(test);
    }

    ///open the file with the unique path
    fstream mail;
    mail.open(path_to_File, fstream::out);

    ///write all information into the mail-txt file.
    mail << "Sender: " << user->getUsername() << endl;
    mail << "Subject: " << subject;

    mail << "attachment: ";
    if(dataLength != 0){mail  << filename << " as " << attachmentFileName;}
    mail << endl;
    mail << message_final;

    ///close the file
    mail.close();

    if(dataLength > 0){
        DIR* test2 = opendir(path_to_attachment_dir.c_str());
        if(test2 == nullptr){
            mkdir(path_to_attachment_dir.c_str(), 0777);
        }
        else{
            closedir(test2);
        }

        ///open the file
        fstream am;
        am.open(path_to_attachment, fstream::out | std::ios::binary);

        ///write all information into the attachment file.
        auto* byteData = new char[dataLength];
        std::copy(data.begin(), data.end(), byteData);

        am.write(byteData, dataLength);
        delete[] byteData;
        ///close the file
        am.close();
    }


    statusMessage = SUCCESS;
    return "Message sent!\n";
}

bool SendMessage::fillMe(string message) {
    ///check if the input from client is just a . and \n, if yes - the input from user is finished
    if(message != ".\n"){
        switch(index){
            case 1:
                ///second information the server needs is the username from the receiver
                if(message.length() <= 9 && message.length() > 1){
                    receiver = message;
                    index ++;
                    statusMessage = "Subject: ";
                    return true;
                }
                statusMessage = "Invalid Receiver - Min 1 Character andi Max 8 Characters";
                index = 1;
                return false;
            case 2:
                ///check if the 2nd input is correct, has to be the subject and not longer as 80 characters
                if(message.length() <= 81){
                    subject = message;
                    index ++;
                    statusMessage = "Attachment (press just ENTER for no attachment) : ";
                    return true;
                }
                statusMessage = "Invalid Subject - Max 80 Characters!";
                index = 1;
                return false;
            case 3:
                ///check if the 3rd input is correct, has to be the filename and not longer then 250 characters
                if (message.length() <= 255 && message != "\n") {
                    attachmentFileName = message;
                    attachmentFileName.pop_back(); // erase \n at end //if filename was given
                    statusMessage = "send_me_this_file: \n" + attachmentFileName;
                    index++;
                } else if (message == "\n") {
                    statusMessage = "Message-Line: ";
                    index += 2; //no need for reading file
                } else {
                    statusMessage = "Invalid filename - Max 255 Characters!";
                    index = 1;
                    return false;
                }
                return true;
            case 4:
                index ++;
                statusMessage = "Message-Line: ";
                return true;
            default:
                ///the last input is the message. At each new line the message gets added to the messages before.
                message_final += message;
                return true;
        }
    }/// check if the index is 4 and all steps are done succesfully - if yes set the server status to "ready of execute" and return true
    else if(index == 5){
        index = 1; //reset index for later send commands
        statusMessage = EXECUTEPENDING;
        return false;
    }else{
        ///if something went wrong in the steps above - return false and give the client the information about invalid input.
        statusMessage = "Invalid Input - Operation cancelled!";
        index = 1;
        return false;
    }
}