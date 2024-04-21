# Secure Chat Application (SCA)
## Introduction
The Secure Chat Application (SCA) provides a strong foundation for safe interactions among users. It emphasizes privacy and reliability, encrypting and securely sending user messages. The platform includes user authentication, comprehensive encryption from start to finish, and an intuitive interface, all designed to enhance the secure and efficient communication experience. By employing RSA encryption algorithms, user credentials are securely stored and transmitted, safeguarding sensitive information from unauthorized access. Additionally, end-to-end encryption ensures that messages can only be decrypted by the intended recipient, maintaining the confidentiality of user communications. Our Secure Chat Application offers a comprehensive solution for secure communication, providing users with the confidence that their messages are protected from interception and tampering.
## Instructions for using the application

![image](https://github.com/AliElgemaey/Secure-Chat-Application-SCA-/assets/114480187/299dca15-80ac-4190-9a23-2363d4c537cb)

Firstly, the user will be asked to enter one of 3 choices
In addition two accounts must be created to communicate with each other.
## Login:
1. Choose option 1 from the main menu.
2. Enter your username, password, and the authorized key associated with the password.
3. Press Enter to confirm.
## SignUp:
1. Choose option 2 from the main menu.
2. Enter your username.
3. Enter Password, Enter password again.
4. Enter Authorized key.
5. Press enter to confirm.
## Exit:
All data will be saved in two files
1. User data will be saved to a file named "users.txt"
2. Messages data will be saved to a file named "messages.txt"
## After account is created Login with your credentials:
1. Chat menu will be visible when logged in.
## Chat menu when logged in
There will be 3 choices to choose from:
1. Send message (Option1)
2. Show Received messages (Option2)
3. Log out (Option3) to go to previous menu (Main Menu) 
## Send Message (Option1)
1. Enter Receiver name (The name of the person you want to send message to)
2. Enter Message
## Show Received Messages (Option2)
All messages sent to the recepient will be visible to them. 
## Logout (Option3)
Go back to the Main menu

## Quick Example
### An account was created using option 2 (SignUp) and logged in
![image](https://github.com/AliElgemaey/Secure-Chat-Application-SCA-/assets/114480187/3ac913e6-83ff-47d7-9b7b-7a55533b6015)

1. Username: Venom
2. Password: Venom_12
3. Authorized key: 6767
4. Login with the credentials of the created account

### Another Account was created and logged in
![image](https://github.com/AliElgemaey/Secure-Chat-Application-SCA-/assets/114480187/94c47d1e-079f-4d60-b5fd-0967905f7c6f)

1. Username: Joker
2. Password: Joker_12
3. Authorized key: 5757

### From the account with username "Venom" the user chose to send a message to "Joker"
![image](https://github.com/AliElgemaey/Secure-Chat-Application-SCA-/assets/114480187/4bdb7544-d54a-4ed3-ad14-cf173f0241b3)

1. So he was asked for the receiver name which was in this case "Joker"
2. Then he entered the message

### After that Login as Joker 
![image](https://github.com/AliElgemaey/Secure-Chat-Application-SCA-/assets/114480187/e7c72435-50a9-49b0-936a-86aafddcee32)

1. After logging in as Joker the user chose option 2 to show recieved messages
2. All messages sent from Venom will be visible

![image](https://github.com/AliElgemaey/Secure-Chat-Application-SCA-/assets/114480187/74d0ec78-0424-46d0-875a-1d74c6379e58)

### When the user finishes and exits (Option3 from Main menu) the application, all data will be saved to files, one  for users and the other for the messages as seen in the image above.

![image](https://github.com/AliElgemaey/Secure-Chat-Application-SCA-/assets/114480187/04f91dde-3025-4e53-9750-3f3e0aa3a83a)

The above image is for the users, as seen the password and authorized keys are encrypted using RSA encryption.

![image](https://github.com/AliElgemaey/Secure-Chat-Application-SCA-/assets/114480187/a81244a1-747f-47a3-888f-81c3de900cd5)

This image is for the messages, as seen it is from Venom to Joker and the messages are encrypted using RSA encryption.
