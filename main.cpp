#include <iostream>
#include <windows.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <cmath>



using namespace std;

struct list
{
    string data = "";
    string value;
    list *head;
};

void PrintList(list *&Beg){
    list * Curr = Beg;
    while(Curr){
        cout << Curr->data << " ";
        Curr = Curr->head;
    }
}
int GetListLength(list *&Beg){
    list *Curr = Beg;
    int i = 0;
    while(Curr){
        ++i;
        Curr= Curr->head;
    }
    return i;
}
void DeleteList ( list * &Beg ){
    list *Next;
    while ( Beg )
    {
        Next = Beg->head;
        delete Beg;
        Beg = Next;
    }
}
list *getAdr(list * &Curr,unsigned index){
    list* beg = Curr;
    while (beg && index--) {
        beg = beg->head;
    }
    return beg;
}
void DelItem( list * &Beg, unsigned Index )
{
    if (Index == 0){
        Beg->data = "";
        return;
    }
    if  ( Index >= GetListLength( Beg ) )
        return;
    list * Item;
    if ( !Index )
    {
        Item = Beg->head;
        delete Beg;
        Beg = Item;
        return;
    }
    Item = getAdr(Beg, Index - 1);
    list * DItem = Item->head;
    Item->head = DItem->head;
    delete DItem;
}


void pushItem(list* &Beg, string userValue, int userIndex){
    list*Curr = Beg;
    if(Curr->data==""){
        Curr->data = userValue;
        return;
    }
    list*Item = new list;
    Item->data = userValue;
    Curr = Beg;
    Item->head = 0;

    while(Curr){
        if(!Curr->head){
            Curr->head = Item;
            return;
        }
        Curr = Curr->head;
    }
}
string reversePolish(list* &act){
    string str,data,polish;
    list *Stack = act;
    getline(cin,str);
    int count =0;
    cout << "\nLine ---- Stack\n";
    for(int i = 0;i<str.length();i++){
        cout <<"    " <<polish << "----";
        PrintList(Stack);
        cout << endl;
        data = str[i];
        if((Stack->data == "") && (str[i]=='/'||str[i]=='*'||str[i]=='+'||str[i]=='-'||str[i]=='(') ){
            pushItem(Stack, data, GetListLength(Stack));
        }
        else {
            switch (str[i]) {
                case '+': case '-':
                    if (getAdr(Stack, GetListLength(Stack)-1)->data != "(") {
                        for (int g = GetListLength(Stack)-1; g >=0; g--) {
                            if(getAdr(Stack, GetListLength(Stack)-1)->data == "("){
                                break;
                            }
                            polish += getAdr(Stack, g)->data;
                            DelItem(Stack, g);

                        }
                    }
                    pushItem(Stack,data, GetListLength(Stack));
                    break;

                case '^':
                    for (int g = GetListLength(Stack)-1; g >=0; g--) {
                        if(getAdr(Stack, g)->data == "("){
                            break;
                        }
                        if (getAdr(Stack, g)->data == "^" ||  getAdr(Stack, g)->data == "cos" || getAdr(Stack, g)->data == "sin" ) {
                            polish += getAdr(Stack, g)->data;
                            DelItem(Stack,g);
                        }
                    }
                    pushItem(Stack,data, GetListLength(Stack));
                    break;
//cos(x)
//cos(45)
                case 'c': case 's':{
                    int g = 1;
                    while (str[i + g] != ')') {
                        data += str[i + g];
                        g++;
                    }
                    polish+=data+')';
                    i += g;
                    break;
                }
                case '*':     case '/':
                    for (int g = GetListLength(Stack)-1; g >=0; g--) {
                        if(getAdr(Stack, g)->data == "("){
                            break;
                        }
                        if (getAdr(Stack, g)->data == "*" ||  getAdr(Stack, g)->data == "/" || getAdr(Stack, g)->data == "^" ||  getAdr(Stack, g)->data == "cos" || getAdr(Stack, g)->data == "sin") {
                            polish += getAdr(Stack, g)->data;
                            DelItem(Stack,g);
                        }
                    }
                    pushItem(Stack,data, GetListLength(Stack));
                    break;

                case '(':
                    pushItem(Stack,data, GetListLength(Stack));
                    break;
                case ')':
                    for (int g = GetListLength(Stack)-1; g >=0; g--) {
                        if (getAdr(Stack, g)->data != "(") {
                            polish += getAdr(Stack, g)->data;
                            DelItem(Stack,g);
                        }
                        else{
                            DelItem(Stack,g);
                            break;
                        }
                    }
                    break;
                default:
                    polish += str[i];
            }
        }
    }
    if(Stack->data!="") {
        for (int g = GetListLength(Stack) - 1; g >= 0; g--) {
            polish += getAdr(Stack, g)->data;
            DelItem(Stack, g);
        }
    }
    cout <<"    " <<polish << "----";
    PrintList(Stack);
    cout << "\n--------------------------------------------------------------\n";
    cout << "\nExpression in Reversed polish form:";
    for (int i = 0; i<polish.length();i++){
        if(polish[i]=='c'|| polish[i]=='s'){
            int g = 0;
            while(polish[i+g]!=')'){
                 cout<<polish[i+g];
                 g++;
            }
            cout << ") ";
            i+=g;
        }
        else if(i==polish.length()-1) {
            cout << polish[i];
        }
        else {
            cout << polish[i] << " ";
        }
    }
    cout << "\n--------------------------------------------------------------\n";

    return polish;
}

bool GetItem(list* &Beg,string userValue){
    list *Curr = Beg;
    while(Curr){
        if(Curr->data==userValue){
            return true;
        }
        Curr = Curr->head;
    }
    return false;
}

double calculationReverse(list* &act,string polish){
    list* Stack = act;
    string data;
    list* var= new list;
    var->head = 0;
    string firstOper, secondOper;
    string userValue;

    float result;
    for(int i = 0; i<polish.length();i++){
        data = polish[i];
        if(polish[i]=='c'||polish[i]=='s'){
            int g = 0;
            while(polish[i+g]!=')'){
                g++;
            }
            i+=g;
        }

        if(polish[i]>=65 && polish[i] <= 90 && (!GetItem(var,data))){

            cout << "\nEnter value of " << polish[i] << " var:\n";
            cin >> userValue;
            data = polish[i];
            pushItem(var,data, GetListLength(var));
            getAdr(var, GetListLength(var)-1)->value = userValue;
        }
    }
    for(int i = 0; i<polish.length();i++){
        stringstream ss;
        string str;
        data = polish[i];
        int varCosin;
        if(polish[i]=='c'||polish[i]=='s'){
            string var = "";
            int g = 4;
            while(polish[i+g]!=')'){
                if(polish[i+g]>=65 && polish[i+g] <= 90){
                    cout << "\nEnter value of " << polish[i+g] << " var(IN DEGREE):\n";
                    cin >> varCosin;
                    var = to_string(varCosin);
//                    break;
                }

                else {
                    var += polish[i + g];
                }
                g++;

            }
            if(polish[i]=='c') {
                float res;
                res = cos((stoi(var)*3.1415926535)/180);
                data = to_string(res);
                pushItem(Stack,data,GetListLength(Stack));

            }
            else {
                float res;
                res = sin((stoi(var)*3.1415926535)/180);
                data = to_string(res);
                pushItem(Stack,data,GetListLength(Stack));

            }

            i+=g;
            //cos(45)
        }
        else if(polish[i]!='+' && polish[i]!='-' && polish[i]!='*' && polish[i]!='/' && polish[i]!='^'){
            pushItem(Stack,data, GetListLength(Stack));
        }
        else{
            firstOper = getAdr(Stack, GetListLength(Stack)-2)->data;
            secondOper = getAdr(Stack, GetListLength(Stack)-1)->data;
//            cout << "OPERATORS " <<firstOper << " "<< secondOper <<endl;
            for(int i = 0;i< GetListLength(var);i++){
                if(firstOper==getAdr(var,i)->data){
                    firstOper = getAdr(var,i)->value;
                }
                if(secondOper==getAdr(var,i)->data){
                    secondOper = getAdr(var,i)->value;
                }
            }
            switch(polish[i]){
                case '+':
                    result = stof(firstOper) + stof(secondOper);
                    ss << result;
                    ss >> str;
                    DelItem(Stack, GetListLength(Stack)-1);
                    DelItem(Stack, GetListLength(Stack)-1);
                    pushItem(Stack,str, GetListLength(Stack));
                    break;
                case '-':
                    result = stof(firstOper) - stof(secondOper);
                    ss << result;
                    ss >> str;
                    DelItem(Stack, GetListLength(Stack)-1);
                    DelItem(Stack, GetListLength(Stack)-1);
                    pushItem(Stack,str, GetListLength(Stack));
                    break;
                case '^':
                    result = pow(stof(firstOper) , stof(secondOper));
                    ss << result;
                    ss >> str;
                    DelItem(Stack, GetListLength(Stack)-1);
                    DelItem(Stack, GetListLength(Stack)-1);
                    pushItem(Stack,str, GetListLength(Stack));
                    break;
                case '*':
                    result = stof(firstOper) * stof(secondOper);
                    ss << result;
                    ss >> str;
                    DelItem(Stack, GetListLength(Stack)-1);
                    DelItem(Stack, GetListLength(Stack)-1);
                    pushItem(Stack,str, GetListLength(Stack));
                    break;
                case '/':
                    result = stof(firstOper) / stof(secondOper);
                    ss << result;
                    ss >> str;
                    DelItem(Stack, GetListLength(Stack)-1);
                    DelItem(Stack, GetListLength(Stack)-1);
                    pushItem(Stack,str, GetListLength(Stack));
                    break;

            }
        }
    }
    cout << "\n--------------------------------------------------------------\n";
    cout <<"Result of calculation from reversed polish form: " <<getAdr(Stack,GetListLength(Stack)-1)->data;
    cout << "\n--------------------------------------------------------------\n";
    DelItem(Stack, GetListLength(Stack)-1);
    DeleteList(var);
}


int main() {
    list *act = new list;
    act->head = 0;
    list *Stack = act;
    string polish = "";
    int cycleMain;
    while (true) {

        cout << "\n--------------------------------------------------------------\n";
        cout << "Choose the command:"
                "\n1 - Convert numeric expression to polish form"
                "\n2 - ‘alculate the expression"
                "\n0 - Exit the programm";
        cout << "\n--------------------------------------------------------------\n";
        cin >> cycleMain;
        if (!cycleMain) {
            DeleteList(act);
            return 0;
        }
        if (cycleMain == 1) {
            system("cls");
            cout << "\n--------------------------------------------------------------\n";
            system("cls");
            cout << "\nEnter the expression:";
            getline(cin, polish);
            polish = reversePolish(Stack);
            cout << "\n--------------------------------------------------------------\n";

        }
        if (cycleMain == 2) {
            if (polish == "") {
                cout << "\n--------------------------------------------------------------\n";
                cout << "Error, you have to enter the expression before calculation";
                cout << "\n--------------------------------------------------------------\n";
            } else {
                system("cls");
                cout << "\nYour expression in Reversed polish form:\n";
                cout << "\n--------------------------------------------------------------\n";
                cout << polish;
                cout << "\n--------------------------------------------------------------\n";
                calculationReverse(Stack, polish);
            }
        }
    }
//    (3*X+5*Y)/(9*X)
//    x^y/(5*z)+10
        Sleep(10000);
        cin.get();
        return 0;

}