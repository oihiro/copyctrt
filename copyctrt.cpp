/*

C++コピーコンストラクタの実験

copyright (C) 2014 Oi Hirokazu All Rights Reserved.

ビルド方法：
on Mac OSX (Mavericks XCode install)
g++ -o copyctrt copyctrt.cpp

参照:Effective C++ 6項
http://www.katto.comm.waseda.ac.jp/~ogura/EffectiveC++_ppt/Section5-8.ppt

2014.06.28結論
・コピーコンストラクタが存在しなければコンパイラによって自動生成される。
・宣言だけしておいて定義しなければ自動生成はされない。(privateに置いた方がよさそう）
・コピーコンストラクタとして何もしない（空の）関数を定義した場合，何もしない動作をする。（自動生成されるコピーコンストラクタとは違い，メンバーはコピーされない）

補足：
コピーコンストラクタやコピー代入演算子を空関数で実装すべきか，宣言だけにすべきか。
コピー禁止を徹底させるNoncopyableクラス : http://marupeke296.com/DXCLS_Noncopyable.html には，空関数で実装する例が載っている。privateにすることで他クラスからのアクセスを禁止している。
しかし，空関数で実装した場合，その実装は定義され，他のメンバ関数やフレンド関数から呼び出すことはできる。だから宣言だけにしたほうがよい。（Effective C++ 6項。本URLの後半でも宣言だけにすべきと改善されている）

*/

#include <iostream>
using namespace std;

class Test1 {
private:
  int a;
public:
  Test1(): a(1) {}
  //Test1(const Test1& t) {} -> コピーコンストラクタが存在しなければコンパイラによって自動生成される。
  int get_a() { return a; }
};

class Test2 {
private:
  int a;
public:
  Test2(): a(1) {}
  Test2(const Test2& t) {} // 自分で定義したコピーコンストラクタ
  int get_a() { return a; }
};

class Test3 {
private:
  int a;
public:
  Test3(): a(1) {}
  Test3(const Test3& t); // 宣言のみの場合，コピーコンストラクタは自動生成されない。publicに宣言した例。
  int get_a() { return a; }
};

class Test3_2 {
private:
  int a;
  Test3_2(const Test3_2& t); // 宣言のみの場合，コピーコンストラクタは自動生成されない。privateに宣言した例。
public:
  Test3_2(): a(1) {}
  int copy_a(const Test3_2& t) { Test3_2 _t = t; return _t.get_a(); } // privateなコピーコンストラクタを使用する
  int get_a() { return a; }
};

/*
Effective C++ 6項ではコピーコンストラクタをコンパイラに自動生成してほしくなければ，privateに宣言だけ置けと書いてあるが，試したところpublicでも生成しなかった。これは処理系依存だから，privateに書いた方がよいということか？（publicに宣言だけしてあって，コンパイラがpublicとして自動生成するのは確かに問題ない仕様に思える)
*/

class Test4: Test3 {
private:
  int a;
public:
  Test4(): a(2) {}
  Test4(const Test4& t) { a = t.a; } // Test3のコピーコンストラクタは自動生成されていなくても，Test3を継承したクラスでコピーコンストラクタを定義できる。（Test3のコピーコンストラクタ宣言がprivate/publicに関わらず）（もちろん，親クラスのコピーコンストラクタを使用していないことが前提）
  int get_a() { return a; }
};


int main() {
  Test1 t1;
  Test1 t1_2 = t1; // 自動生成されたコピーコンストラクタの呼び出し。a=1がコピーされる。
  cout << "t1_2.a = " << t1_2.get_a() << endl;
  
  Test2 t2;
  Test2 t2_2 = t2; // 自分で定義したコピーコンストラクタの呼び出し。メンバをコピーしていないので，aの値は不定になる。不定かどうかは処理系依存かもしれない。
  cout << "t2_2.a = " << t2_2.get_a() << endl;

  Test3 t3;
  //Test3 t3_2 = t3; // publicコピーコンストラクタがないので，この行を有効化すると，リンクエラーになる。

  Test3_2 t3_2_1;
  //int t3_2_1_a = t3_2_1.copy_a(t3_2_1); // privateコピーコンストラクタがないので，この行を有効化すると，リンクエラーになる。

  Test4 t4;
  Test4 t4_2 = t4;
  cout << "t4_2.a = " << t4_2.get_a() << endl;

  return 0;
}

