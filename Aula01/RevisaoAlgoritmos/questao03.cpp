#include <iostream>

using namespace std;

void contanum (int vetor[])
{
	int i = 0;
	int pares = 0;
	int impares = 0;
	int positivos = 0;
	int negativos = 0;

	while(i != 5)
	{
		if(vetor[i] >= 0)
		{
			positivos++;
			if(vetor[i] % 2 == 0)
				pares++;
			else
				impares++;
		}
		else{
			negativos++;
			if(vetor[i] % 2 == 0)
				pares++;
			else
				impares++;
		}
		i++;
	}	
	cout << "Quantidade de numeros pares: " << pares << endl;
	cout << "Quantidade de numeros impares: " << impares << endl;
	cout << "Quantidade de numeros positivos: " << positivos << endl;
	cout << "Quantidade de numeros negativos: " << negativos << endl;
	cout << endl;
}
int main()
{
	int vetor[5];
	int n;
	int i, j;

	cin >> n;

	for(i = 0; i < n; i++)
	{
		j = 0;
		while(j != 5)
		{
			cin >> vetor[j];
			j++;
		}	
		contanum(vetor);
	}
	
	return 0;
}
