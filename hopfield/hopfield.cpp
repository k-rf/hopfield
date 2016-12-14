#include <iostream>
#include <fstream>

using namespace std;

// 重み行列Wをパターンで初期化
void weights(int** W, int* x0, int* x1, int* x2, int N)
{
	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++)
			W[i][j] = (x0[i] * x0[j]) + (x1[i] * x1[j]) + (x2[i] * x2[j]);
	for(int k = 0; k < N; k++)
		W[k][k] = 0;
}

// 和の計算
void mul(int** W, int* s, int* h, int N)
{
	for(int i = 0; i < N; i++)
	{
		int sum = 0;
		for(int j = 0; j < N; j++)
			sum += W[i][j] * s[j];
		h[i] = sum;
	}
}

int sign(int y) { return (y > 0) ? 1 : 0; }

// 2つのベクトルが同じかどうかのチェックをする
int check(int* v1, int* v2, int N)
{
	for(int i = 0; i < N; i++)
		if(v1[i] != v2[i])
			return 0;
	return 1;
}

// energy of the configuration Ising model
int energy(int** W, int* s, int N)
{
	int E = 0;
	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++)
			E += W[i][j] * s[i] * s[j];
	return -E;
}

// =======================================================================
// main関数
// =======================================================================
int main()
{
	ifstream ifs;
	int N = 40;
	int* x0 = new int[N];
	int* x1 = new int[N];
	int* x2 = new int[N];

	// pattern 0 ==> "1"
	ifs.open("0.txt");
	for(int i = 0; i < N; i++)
		ifs >> x0[i];
	ifs.close();

	// pattern 1 ==> "2"
	ifs.open("1.txt");
	for(int i = 0; i < N; i++)
		ifs >> x1[i];
	ifs.close();

	// pattern 2 ==> "4"
	ifs.open("2.txt");
	for(int i = 0; i < N; i++)
		ifs >> x2[i];
	ifs.close();

	int** W = NULL;
	W = new int*[N];
	for(int i = 0; i < N; i++)
		W[i] = new int[N];

	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++)
			W[i][j] = 0;

	weights(W, x0, x1, x2, N);

	int* s = new int[N];

	// start configuration
	ifs.open("s.txt");
	for(int i = 0; i < N; i++)
		ifs >> s[i];
	ifs.close();

	int E = energy(W, s, N);
	cout << "energy of initial configuration: " << E << endl;

	int* h = new int[N];
	for(int p = 0; p < N; p++)
		h[p] = 0;

	int* s1 = new int[N];

	for(int i = 0; i < N; i++)
		s1[i] = s[i];
	int result, count;
	count = 0;

	do
	{
		for(int i = 0; i < N; i++)
			s1[i] = s[i];
		mul(W, s, h, N);

		for(int j = 0; j < N; j++)
		{
			if(h[j] != 0) { s[j] = sign(h[j]); }
			if(h[j] == 0) { s[j] = s1[j]; }
		}
		result = check(s, s1, N);
		count++;
		cout << "count = " << count << endl;
	} while((count < 100) && (result != 1));

	cout << "number of iterations: " << count << endl;

	for(int i = 0; i < N; i++)
	{
		if(s[i] == 0) { cout << "##"; }
		if(s[i] == 1) { cout << "  "; }
		//cout << " "; // end configuration
		if(((i + 1) % 5) == 0) { cout << endl; }
	}

	E = energy(W, s, N);
	cout << "energy of end configuration: " << E << endl;

	delete[] x0;
	delete[] x1;
	delete[] x2;
	delete[] s;
	delete[] s1;
	delete[] h;

	for(int i = 0; i < N; i++) { delete[] W[i]; }
	delete[] W;

	while(getchar() != 'q');
	return 0;
}
