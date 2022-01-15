#include <crypt.h>
#include <string>
#include <iostream>
#include <fstream>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include<bits/stdc++.h>
using namespace std;

//Returns the salt portion from the full encrypted hash string
string getSalt(string hash)
{
    return hash.substr(0, 12);
}
//calculates the hash for the generated password and compares it to the existing hash
bool checkHash(string pass, string salt, string target)
{
    string hash = crypt(pass.c_str(), salt.c_str());
    if (hash == target) {return true;}
    else {return false;}
}

//Main function where MPI is to be implemented
string decryptPass(string encryptedHash,int task_id) 
{


    string salt = getSalt(encryptedHash);
    string password = "a```````";
    int pid = task_id; 
    //each process will get relevant letter. depends on total process count
    int size =25;
    if(pid == 0)
    {
        password="`_______";
        size=26;
    }
    else{
        for (int i=1;i<=26;i++){
            if(pid == i){
                password[0] += pid-1;
                break;
            }
        }       
    }

    //The bellow for loop itterates and randomly generates the password, starting from aaaaaaaa to aaaaaaab and so on, each password is tested agains't the hashsudo
    int i1 = 0;
    while (i1 < size)
    {
        password[1] += 1;
        int i2 = 0; 
        while (i2 < size)
        {
            password[2] += 1;
            int i3 = 0; 
            while (i3 < size)
            {
                password[3] += 1;
                int i4 = 0; 
                while (i4 < size)
                {
                    password[4] += 1;
                    int i5 = 0; 
                    while (i5 < size)
                    {
                        password[5] += 1;
                        int i6 = 0; 
                        while (i6 < size)
                        {
                            password[6] += 1;
                            int i7 = 0; 
                            while (i7 < size)
                            {
                                password[7] += 1;
                                string temp="";
                               if(pid==0){
                                   int i=0;
                                while( i<8){
                                    if(password[i]=='`')
                                    {}
                                    else{
                                    temp=temp+password[i];
                                    
                                    }
                                    i++;
                                }
                                cout << temp << endl;
                               
                               
                                if (checkHash(temp, salt, encryptedHash))
                                {
                                    return temp;
                                }
                                
                               }
                        
                                                
                                if (checkHash(password, salt, encryptedHash))
                                {
                                    return password;
                                }
                                i7++;
                            }
                            password[7] = '`';
                            i6++;
                        }
                        password[6] = '`';
                        i5++;
                    }
                    password[5] = '`';
                    i4++;
                }
                password[4] = '`';
                i3++;
            }
            password[3] = '`';
            i2++;
        }
        password[2] = '`';
        i1++;
    }
    return "n";
}
string getHash(string username){
    ifstream MyReadFile("shadow.txt");
    string myText;
    vector <string> tokens;
        // Use a while loop together with the getline() function to read the file line by line
    while (getline(MyReadFile, myText)) {
        
        if (myText.find(username) != std::string::npos) {

            stringstream check1(myText);

            string intermediate;

            // Tokenizing w.r.t. space ' '
            while (getline(check1, intermediate, ':'))
            {
                tokens.push_back(intermediate);
            }
            std::cout << "found!" << tokens[1]<<'\n';
            break;
        }
        
    }


    // Close the file
    MyReadFile.close();
    return tokens[1];
}
int main(int argc, char *argv[])
{
   
    string userid;
    string hash;
    int soze;
    int taskid,numtasks,numslaves;
    int err;
    int dest;


    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    numslaves = numtasks-1;
    string receivehash;
    char buffer[1000];
    if(taskid==0)
    {

        hash=getHash("farrukh");
        soze=hash.size();
        for(dest=1; dest<=numslaves; dest++)
        {         
            MPI_Send(&soze, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            MPI_Send(hash.c_str(), hash.size(), MPI_CHAR, dest, 0, MPI_COMM_WORLD);
    
        }

        string pass = decryptPass(hash,taskid);

        if (pass == "-")
        {
            cout << "Not found Taskid "<<taskid<<"\n";
        }
        else
        {
            cout << "Password found: " <<pass<<" by Task id "<<taskid<<"\n";
            MPI_Abort(MPI_COMM_WORLD,err);
        }
    }
    else
    {
        MPI_Recv(&soze, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&buffer, soze, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        buffer[98]='\0';
        string hassha(buffer);
    
        cout<<hassha<<"\n";
        string pass = decryptPass(hassha,taskid);
        if (pass == "-")
        {
            cout << "Not found Taskid "<<taskid<<"\n";
        }
        else
        {
            cout << "Password found: " <<pass<<" by Task id "<<taskid<<"\n";
            MPI_Abort(MPI_COMM_WORLD,err);
        }
    }
    MPI_Finalize();
    
}   
