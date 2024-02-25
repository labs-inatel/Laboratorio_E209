#include<iostream>
#include<iomanip>

using namespace std;

int main()
{
	int quantidade;
	float total;

	cin >> quantidade;

	if(quantidade >= 12)
	{
		cout << "Preco da unidade: R$0.65" << endl;
		total = quantidade * 0.65;
	}
	else
	{
		cout << "Preco da unidade: R$0.80" << endl;
		total = quantidade * 0.80;
	}
	
	cout << fixed << setprecision(2);
	cout << "Preco total: R$" << total << endl;

	return 0;
}
