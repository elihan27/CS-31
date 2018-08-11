#include <iostream>
#include <string>
using namespace std;

int main ()
{
    const double FIRST_TWENTY = 20 * .181; // value of license fee for first $20 million
    
    //gathering data
    cout << "Identification: " ;
    string personID;
    getline (cin, personID) ;
    
    
    cout << "Expected revenue (in millions): " ;
    double revenue;
    cin >> revenue;
    cin.ignore(1000, '\n');
    
    
    cout << "Country: " ;
    string country;
    getline (cin, country);
    
    cout << "---" << endl;
    
    
    // accounting for different license fee percentages when revenue is over 20 and over fifty
    double nextThirty = revenue - 20;
    double overFifty = revenue - 50;
    double licenseFee;
    
    //accounting for different license fee percentages when revenue and country are different
    
    if (revenue <= 20)
        licenseFee = revenue * .181;
    else if (revenue <= 50 && (country == "Turkey" || country == "UAE"))
        licenseFee = (nextThirty * .217) + FIRST_TWENTY;
    else if (revenue <= 50 && (country != "Turkey" && country != "UAE"))
        licenseFee = (nextThirty * .203) + FIRST_TWENTY;
    else if (revenue > 50 && (country == "Turkey" || country == "UAE"))
        licenseFee = FIRST_TWENTY + (30 * .217) + (overFifty * .23);
    else if (revenue > 50 && (country != "Turkey" && country != "UAE"))
        licenseFee = FIRST_TWENTY + (30 * .203) + (overFifty * .23);
    else licenseFee = 0;
    
    cout.setf(ios::fixed); // three decimal points
    cout.precision(3);
    
    //all the different output statements
    
    if (personID != "" && revenue >= 0 && country != "")
        cout << "The license fee for " << personID << " is $" << licenseFee << " million.";
    else if (personID == "")
        cout << "You must enter a property identification." << endl;
    else if (revenue < 0)
        cout << "The expected revenue must be nonnegative." << endl;
    else if (country == "")
        cout << "You must enter a country." << endl;
    
    
    
}
