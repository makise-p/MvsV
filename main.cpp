
#include<iostream>
#include<string>
#include<random>
#include<fstream>
#include<vector>
#include<algorithm>

using namespace std;

int damage = 0;

vector< string > things;//持っているアイテムを管理

//乱数作成
int p_damage(){
	random_device rnd;
	mt19937 mt(rnd());
	uniform_int_distribution<> rand2001(11000, 13000);
	return rand2001(mt);
}

int e_damage(){
	random_device rnd;
	mt19937 mt(rnd());
	uniform_int_distribution<> rand1501(500, 2000);
	return rand1501(mt);
}

struct person
{
	person(const int id_, const std::string& call_) : id(id_), call(call_) { }
	bool operator<(const person& obj) const { return (call < obj.call); }
	bool operator>(const person& obj) const { return (call > obj.call); }
	bool operator==(const person& obj) const { return (call == obj.call); }
	bool operator!=(const person& obj) const { return !(*this == obj); }

	int id;
	std::string call;
};

//クラステンプレート使ってみる
//template<class T>
class Character{
public:
	Character(string _name, int i){
		name = _name;
		hp = i; //HPの定義
	}

	bool AttackFlag;
	bool DefenseFlag;
	bool ItemFlag;

	string get_name() const{ return name; } //名前を渡す
	int get_hp() { return hp; }	//体力を渡す
	int set_hp(int _hp){
		hp = _hp;
		return hp;
	}

	void think(){
		string doing;
		cout << '\n';
		cout << "攻撃:attack、防御:defense、アイテム:item" << endl;

		//行動のコマンド入力処理
		while (true){

			//次のコマンドのために初期化
			AttackFlag = false;	
			DefenseFlag = false;
			ItemFlag = false;

			cout << "command:";
			cin >> doing;
			if (!cin.fail()){ //エラー処理
				if (doing == "attack"){
					damage = attack();
					AttackFlag = true;
					break;
				}
				else if (doing == "defense"){
					damage = defense();
					DefenseFlag = true;
					break;
				}
				else if (doing == "item"){
					ItemFlag = true;
					break;
				}
				else{
					cout << "入力ミス！" << endl;
					cout << "攻撃ならattack、防御ならdefenseを入力してください" << endl;
					continue;
				}
			}//cin.fail
			cin.clear();
			cin.seekg(0);
		}
	}

	virtual int attack(){
		return 0;
	};

	virtual int defense(){
		return 0;
	};
	virtual void item(){};

private:
	string name;
	int hp;
};

class Player:public Character{
public:
	Player():Character("Makise", 7000)
	{
	}
	
	int attack(){
		return p_damage();
	}

	int defense(){
		return 0;
	}

	void item(){

	}
};

class Enemy :public Character{
public:
	Enemy() :Character("Veera", 150000)
	{
	}
	int attack(){
		return e_damage();
	}
};
/*
class Collector{
public:
	string name;
	int hp;
	Collector(string _name, int _hp){
		name = _name;
		hp = _hp;
	}

	int hp(int damage){
		hp = hp - damage;
		return hp;
	}
};
*/
class Game{
public:
	int turn; //ターン管理
	string Winner;

	void ResultWriting(int count){
		ofstream ofs("ResultRecode.txt", ios::app);
		if (count == 1)
			ofs << "今回の起動での戦闘" << endl;
		ofs << '\t' << count << "回目の戦闘記録" << endl;
		ofs << '\t' << "戦闘は" << turn << "ターン" << endl;
		ofs << '\t' << "勝者は" << Winner << '\n' << endl;
		ofs.close();
	}

	Game(){
		turn = 0;
	}

	void start(){
		Player player;
		Enemy enemy;
		//Collector pl("Makise", 7000);
		//Collector ene("Veera", 150000);

		//名前を表示
		cout << enemy.get_name() << ':' << enemy.get_hp();
		cout << '\t';
		cout << player.get_name() << ':' << player.get_hp() << endl;
		cout << "戦闘開始" << endl;

		bool FinishFlag = false;	//勝者が決まったか判断
		bool DefenseModeFlag = false;
		bool OnceMoreFlag = false;
		bool MeroMeroFlag = false;
		int MeroMeroCount = -1;	//そもそもアイテムを使わないとFlagが立たないから-1だと未使用、0だと使用済みということにする
		int potion = 1;

		while (true){
			if (OnceMoreFlag == false){	//item選択で使用しない場合もう一度行動選択するため
				//ターン処理
				turn += 1;
				cout << '\n';
				cout << "ターン" << turn << endl;
			}
			//自分の攻撃処理
			player.think();
			if (player.AttackFlag == true){
				cout << player.get_name() << "は" << enemy.get_name() << "に" << damage << "のダメージを与えた！！" << endl;
				DefenseModeFlag = false;
				OnceMoreFlag = false;	//アイテム使用後でも攻撃できるため、攻撃したらターンを進める
			}

			//自分の防御処理
			else if (player.DefenseFlag == true){
				cout << player.get_name() << "は身を守っている" << endl;
				DefenseModeFlag = true;
				OnceMoreFlag = false;	//アイテム使用後でも防御できるため、防御したらターンを進める
			}

			//自分のアイテム選択・使用処理
			else if (player.ItemFlag == true){
				while (true){	//アイテム使用選択画面
					cout << '\n';
					cout << "何を使いますか？" << endl;
					//持っているアイテムを表示
					int num = 0;
					vector< string >::iterator item = things.begin();
					while (item != things.end()){
						cout << num << ":" << *item << endl;
						++num;
						++item;
					}

/*					for (int i = 0; i < things.size(); i++)
						cout << i << ":" << things[i] << endl;
*/
					cout << "やめておく場合は99と入力" << endl;
					int number = 7;
					while (true){
						cout << "number:";
						cin >> number;
						if (!cin.fail()){ //エラー処理
							if (number == 1){
								if (potion == 0){
									cout << "もうポーションはありません" << endl;
									continue;
								}
								else{
									cout << player.get_name() << "の体力が3000回復します" << endl;
									break;
								}
							}
							else if (number == 0){
								if (MeroMeroCount > 0){
									cout << "カタリナさん人形はヴィーラが幸せそうに使っている！！" << endl;
									continue;
								}
								else if (MeroMeroCount == 0){
									cout << "カタリナさん人形は見るも無残な姿になっている…！！！" << endl;
									continue;
								}
								else{
									cout << "使ってからのお楽しみ♪" << endl;
									break;
								}
							}
							else if (number == 99){ //アイテムを使わないためコマンド入力まで戻る
								OnceMoreFlag = true;
								break;
							}
						}//cin.fail()終わり
						cin.clear();
						cin.seekg(0);
					}
					if (OnceMoreFlag == true)
						break;//下のコマンド入力へ行くcontinueへ
					else if ( OnceMoreFlag == false  || potion != 0 ){
						cout << "使いますか？ yes/no" << endl;
						string decision;
						while (true){
							cin >> decision;
							if (!cin.fail()){ //エラー処理
								if (decision == "yes" || decision == "y"){
									OnceMoreFlag = false;
									break;
								}
								else if (decision == "no" || decision == "n"){
									OnceMoreFlag = true;
									break;
								}
								else{
									cout << "入力ミス！" << endl;
									cout << "使うならyes/y、使わないならno/nを入力してください" << endl;
									continue;
								}
							}//cin.fail終わり
							cin.clear();
							cin.seekg(0);
						}
					}
					else if (potion == 0){
						continue;//アイテム選択へ
					}
					if (OnceMoreFlag == true)
						continue;//アイテム選択へ
					else{
						cout << player.get_name() << "は" << things[number] << "を使った" << endl;
						//アイテム使用時の処理
						if (number == 1){
							if (player.get_hp() <= 4000){//プレイヤーの行動ができるため体力は0以上が確定　のためはしょる
								player.set_hp(player.get_hp() + 3000);
								cout << player.get_name() << "は3000回復した！" << endl;
								cout << player.get_name() << "の体力は" << player.get_hp() << endl;
								potion = potion - 1;
							}
							else {
								player.set_hp(7000);
								cout << player.get_name() << "は全回復した！" << endl;
								cout << player.get_name() << "の体力は" << player.get_hp() << endl;
								potion = potion - 1;
							}
							item = things.begin();	//	イテレータで削除
							++item;
							item = things.erase(item);
						}
						else if (number == 0){
							cout << enemy.get_name() << "はCatalina dollにメロメロだ！！！" << endl;
							MeroMeroCount = 3;
							MeroMeroFlag = true;
						}
					}
				}
				if (OnceMoreFlag == true)
					continue;//コマンド入力へ
			}

			enemy.set_hp(enemy.get_hp() - damage);//ダメージ計算

			//敵の生死を確認
			if (enemy.get_hp() > 0)
				cout << enemy.get_name() << "のHPは残り" << enemy.get_hp() << endl;
			else {
				cout << player.get_name() << "は" << enemy.get_name() << "を倒した！！" << endl;
				FinishFlag = true;
				Winner = player.get_name();
				break;
			}
			//敵の攻撃処理
			damage = enemy.attack();
			cout << enemy.get_name() << "の攻撃" << endl;
			if (MeroMeroFlag == true){	//メロメロ状態
				cout << enemy.get_name() << "はCatalina dollに夢中なようだ…" << endl;
				MeroMeroCount -= 1;
				if (MeroMeroCount == 0)
					MeroMeroFlag = false;
			}
			else if (DefenseModeFlag != true) {
				player.set_hp(player.get_hp() - damage);
				cout << player.get_name() << "は" << damage << "ダメージをくらった" << endl;
			}
			else if (DefenseModeFlag == true) {
				player.set_hp(player.get_hp() - (damage / 2));
				cout << player.get_name() << "は" << damage / 2 << "ダメージをくらった" << endl;
			}
			

			//自分の生死を確認
			if (player.get_hp() > 0)
				cout << player.get_name() << "のHPは残り" << player.get_hp() << endl;
			else {
				cout << player.get_name() << "は倒れた" << endl;
				FinishFlag = true;
				Winner = enemy.get_name();
				break;
			}
		}
	}

};

int main(){
	Game game;
	
	things.push_back("Catalina doll");	//Charactorクラス内ではエラーが出た
	things.push_back("potion");

	const string calls[5] = { "Veera", "Veera", "Veera", "Catalina", "Veera" };
	vector<person> p;
	for (size_t i = 0; i < 5; ++i){ p.push_back(person(i, calls[i])); }
	sort(p.begin(), p.end());
	person v(5, "Veera");

	vector<person>::iterator per = find(p.begin(), p.end(), v);
	if (per != p.end())
	{
		cout << per->id << ": found" << endl;
		cout << '\n';
	}

	game.start();
	int GameCount = 1;
	game.ResultWriting(GameCount);
	GameCount = 2;

	string decision;
	while (true){
		cout << '\n';
		cout << "再戦する？ yes/no" << endl;
		cout << "decision:";
		cin >> decision;
		if (!cin.fail()){ //エラー処理
			if (decision == "yes" || decision == "y"){
				game.turn = 0;
				game.start();
				game.ResultWriting(GameCount);
				GameCount += 1;
				continue;
			}
			else if (decision == "no" || decision == "n"){
				break;
			}
			else{
				cout << "入力ミス！" << endl;
				cout << "再戦ならyes/y、闘いを終えてリアルに戻るならno/nを入力してください" << endl;
				continue;
			}
		}//cin.fail終わり
		cin.clear();
		cin.seekg(0);
	}

	return 0;
}

