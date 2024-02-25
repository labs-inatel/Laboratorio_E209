#include <iostream>

using namespace std;

int main()
{
	int n;
	int total;
	int i;
	int aux;

	cin >> n;

	aux = n;

	for(i = 0; i < n; i++)
	{
		if(aux != 1)
		{
			total = n % (aux - 1);
			aux--;
			cout << "Resto da divisao de " << n << " por " << aux << " : " << total << endl;
		}	
	}
	return 0;
}
