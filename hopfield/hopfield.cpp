#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <random>

using namespace std;

const int w = 10; 
const int h = 18;
const int n = 7;
const double rate = 0.2;

// 画像の配列サイズ(5 x 8)
const int N = h * w;

int inputIntFrom(int first, int last)
{
	while(true)
	{
		string tmp;
		getline(cin, tmp); // 区切り文字を無効にするためにcinではなくgetlineを使用する

		stringstream ss(tmp);
		tmp.clear(); // 再利用するため

		int time = first - 1;
		ss >> time >> tmp;

		if(time >= first && time <= last && tmp == "") { return time; }
		else { cout << "正しい数値を入力してください．\n> "; }
	}
}

// 重み行列Wをパターンで初期化
void weights(int** W, int** x, int n)
{
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			for(int k = 0; k < n; k++)
			{
				W[i][j] += (x[k][i] * x[k][j]);
			}
		}
	}
	for(int k = 0; k < N; k++)
		W[k][k] = 0;
}

// 和の計算
void mul(int** W, int* s, int* h)
{
	for(int i = 0; i < N; i++)
	{
		int sum = 0;
		for(int j = 0; j < N; j++)
			sum += W[i][j] * s[j];
		h[i] = sum;
	}
}

int sign(int y) 
{
	if(y > 0) return 1;
	else return -1;
}

// 2つのベクトルが同じかどうかのチェックをする
int check(int* v1, int* v2)
{
	for(int i = 0; i < N; i++) { if(v1[i] != v2[i]) { return 0; } }
	return 1;
}

// energy of the configuration Ising model
int energy(int** W, int* s)
{
	int E = 0;
	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++)
			E += W[i][j] * s[i] * s[j];
	return -E;
}

// ノイズを付与
void noise(int* state)
{
	const int Noise = N * rate;
	
	int i, j;
	for(i = 0; i < Noise; i++)
	{
		j = rand() % N;
		if(state[j] == -1) { state[j] = 1; }
		else { state[j] = -1; }
	}
}

// =======================================================================
// main関数
// =======================================================================
int main()
{
	ifstream ifs;

	int** x = new int*[n];
	for(int i = 0; i < n; i++) { x[i] = new int[N]; }

	string in(".txt");
	for(int i = 0; i < n; i++)
	{
		in.insert(0, to_string(i));
		ifs.open(in);
		char c;
		for(int j = 0; j < N; j++) 
		{
			ifs >> x[i][j] >> c;
			if(x[i][j] == 255) { x[i][j] = 1; }
			else { x[i][j] = -1; }
		}
		ifs.close();
		in.erase(0, 1);
	}

	int** W = NULL;
	W = new int*[N];
	for(int i = 0; i < N; i++) { W[i] = new int[N]; }

	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++)
			W[i][j] = 0;

	weights(W, x, n);

	int* s = new int[N];
	
	
	// start configuration
	//ifs.open("s1.txt");
	//for(int i = 0; i < N; i++) { ifs >> s[i]; }
	//ifs.close();

	bool loop = true;
	while(loop)
	{
		random_device rand;
		int* st = new int[N];

		int xi = rand() % n;
		for(int i = 0; i < N; i++) { st[i] = x[xi][i]; }
		noise(st);
		for(int i = 0; i < N; i++) { s[i] = st[i]; }
		//for(int i = 0; i < N; i++) { st[i] = s[i]; }


		int E = energy(W, s);
		cout << "energy of initial configuration: " << E << endl;

		int* h = new int[N];
		for(int p = 0; p < N; p++) { h[p] = 0; }

		int* s1 = new int[N];

		for(int i = 0; i < N; i++) { s1[i] = s[i]; }

		int result, count;
		count = 0;

		do
		{
			for(int i = 0; i < N; i++) { s1[i] = s[i]; }
			mul(W, s, h);

			for(int j = 0; j < N; j++)
			{
				if(h[j] != 0) { s[j] = sign(h[j]); }
				if(h[j] == 0) { s[j] = s1[j]; }
			}
			result = check(s, s1);
			count++;
			cout << "count = " << count << endl;
		} while((count < 1000) && (result != 1));

		cout << "number of iterations: " << count << endl;

		cout << "\n--------------------------- 入力 ---------------------------\n\n";
		cout << "　　　　　　　　　　";
		for(int i = 0; i < N; i++)
		{
			if(st[i] == 1) { cout << "■"; }
			if(st[i] == -1) { cout << "　"; }
		
			if(((i + 1) % w) == 0) { cout << endl; cout << "　　　　　　　　　　"; }
		}

		cout << "\n\n------------------------- 判定結果 -------------------------\n\n";
		cout << "　　　　　　　　　　";
		for(int i = 0; i < N; i++)
		{
			if(s[i] == 1) { cout << "■"; }
			if(s[i] == -1) { cout << "　"; }
			//cout << " "; // end configuration
			if(((i + 1) % w) == 0) { cout << endl; cout << "　　　　　　　　　　"; }
		}
		cout << "\n";

		E = energy(W, s);
		cout << "energy of end configuration: " << E << endl;

		cout << "1 : 繰り返し，0 : 終了" << endl;
		loop = static_cast<bool>(inputIntFrom(0, 1));

		//delete[] s1;
		//delete[] h;

	}

	//delete[] x[0];
	//delete[] x[1];
	//delete[] x[2];
	//delete[] x[3];
	//delete[] s;

	//for(int i = 0; i < N; i++) { delete[] W[i]; }
	//delete[] W;

//	while(getchar() != 'q');
	return 0;
}
