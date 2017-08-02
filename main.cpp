
#include<iostream>
#include<string>
#include<random>
#include<fstream>
#include<vector>
#include<algorithm>

using namespace std;

int damage = 0;

vector< string > things;//�����Ă���A�C�e�����Ǘ�

//�����쐬
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

//�N���X�e���v���[�g�g���Ă݂�
//template<class T>
class Character{
public:
	Character(string _name, int i){
		name = _name;
		hp = i; //HP�̒�`
	}

	bool AttackFlag;
	bool DefenseFlag;
	bool ItemFlag;

	string get_name() const{ return name; } //���O��n��
	int get_hp() { return hp; }	//�̗͂�n��
	int set_hp(int _hp){
		hp = _hp;
		return hp;
	}

	void think(){
		string doing;
		cout << '\n';
		cout << "�U��:attack�A�h��:defense�A�A�C�e��:item" << endl;

		//�s���̃R�}���h���͏���
		while (true){

			//���̃R�}���h�̂��߂ɏ�����
			AttackFlag = false;	
			DefenseFlag = false;
			ItemFlag = false;

			cout << "command:";
			cin >> doing;
			if (!cin.fail()){ //�G���[����
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
					cout << "���̓~�X�I" << endl;
					cout << "�U���Ȃ�attack�A�h��Ȃ�defense����͂��Ă�������" << endl;
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
	int turn; //�^�[���Ǘ�
	string Winner;

	void ResultWriting(int count){
		ofstream ofs("ResultRecode.txt", ios::app);
		if (count == 1)
			ofs << "����̋N���ł̐퓬" << endl;
		ofs << '\t' << count << "��ڂ̐퓬�L�^" << endl;
		ofs << '\t' << "�퓬��" << turn << "�^�[��" << endl;
		ofs << '\t' << "���҂�" << Winner << '\n' << endl;
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

		//���O��\��
		cout << enemy.get_name() << ':' << enemy.get_hp();
		cout << '\t';
		cout << player.get_name() << ':' << player.get_hp() << endl;
		cout << "�퓬�J�n" << endl;

		bool FinishFlag = false;	//���҂����܂��������f
		bool DefenseModeFlag = false;
		bool OnceMoreFlag = false;
		bool MeroMeroFlag = false;
		int MeroMeroCount = -1;	//���������A�C�e�����g��Ȃ���Flag�������Ȃ�����-1���Ɩ��g�p�A0���Ǝg�p�ς݂Ƃ������Ƃɂ���
		int potion = 1;

		while (true){
			if (OnceMoreFlag == false){	//item�I���Ŏg�p���Ȃ��ꍇ������x�s���I�����邽��
				//�^�[������
				turn += 1;
				cout << '\n';
				cout << "�^�[��" << turn << endl;
			}
			//�����̍U������
			player.think();
			if (player.AttackFlag == true){
				cout << player.get_name() << "��" << enemy.get_name() << "��" << damage << "�̃_���[�W��^�����I�I" << endl;
				DefenseModeFlag = false;
				OnceMoreFlag = false;	//�A�C�e���g�p��ł��U���ł��邽�߁A�U��������^�[����i�߂�
			}

			//�����̖h�䏈��
			else if (player.DefenseFlag == true){
				cout << player.get_name() << "�͐g������Ă���" << endl;
				DefenseModeFlag = true;
				OnceMoreFlag = false;	//�A�C�e���g�p��ł��h��ł��邽�߁A�h�䂵����^�[����i�߂�
			}

			//�����̃A�C�e���I���E�g�p����
			else if (player.ItemFlag == true){
				while (true){	//�A�C�e���g�p�I�����
					cout << '\n';
					cout << "�����g���܂����H" << endl;
					//�����Ă���A�C�e����\��
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
					cout << "��߂Ă����ꍇ��99�Ɠ���" << endl;
					int number = 7;
					while (true){
						cout << "number:";
						cin >> number;
						if (!cin.fail()){ //�G���[����
							if (number == 1){
								if (potion == 0){
									cout << "�����|�[�V�����͂���܂���" << endl;
									continue;
								}
								else{
									cout << player.get_name() << "�̗̑͂�3000�񕜂��܂�" << endl;
									break;
								}
							}
							else if (number == 0){
								if (MeroMeroCount > 0){
									cout << "�J�^���i����l�`�̓��B�[�����K�������Ɏg���Ă���I�I" << endl;
									continue;
								}
								else if (MeroMeroCount == 0){
									cout << "�J�^���i����l�`�͌�������c�Ȏp�ɂȂ��Ă���c�I�I�I" << endl;
									continue;
								}
								else{
									cout << "�g���Ă���̂��y���݁�" << endl;
									break;
								}
							}
							else if (number == 99){ //�A�C�e�����g��Ȃ����߃R�}���h���͂܂Ŗ߂�
								OnceMoreFlag = true;
								break;
							}
						}//cin.fail()�I���
						cin.clear();
						cin.seekg(0);
					}
					if (OnceMoreFlag == true)
						break;//���̃R�}���h���͂֍s��continue��
					else if ( OnceMoreFlag == false  || potion != 0 ){
						cout << "�g���܂����H yes/no" << endl;
						string decision;
						while (true){
							cin >> decision;
							if (!cin.fail()){ //�G���[����
								if (decision == "yes" || decision == "y"){
									OnceMoreFlag = false;
									break;
								}
								else if (decision == "no" || decision == "n"){
									OnceMoreFlag = true;
									break;
								}
								else{
									cout << "���̓~�X�I" << endl;
									cout << "�g���Ȃ�yes/y�A�g��Ȃ��Ȃ�no/n����͂��Ă�������" << endl;
									continue;
								}
							}//cin.fail�I���
							cin.clear();
							cin.seekg(0);
						}
					}
					else if (potion == 0){
						continue;//�A�C�e���I����
					}
					if (OnceMoreFlag == true)
						continue;//�A�C�e���I����
					else{
						cout << player.get_name() << "��" << things[number] << "���g����" << endl;
						//�A�C�e���g�p���̏���
						if (number == 1){
							if (player.get_hp() <= 4000){//�v���C���[�̍s�����ł��邽�ߑ̗͂�0�ȏオ�m��@�̂��߂͂����
								player.set_hp(player.get_hp() + 3000);
								cout << player.get_name() << "��3000�񕜂����I" << endl;
								cout << player.get_name() << "�̗̑͂�" << player.get_hp() << endl;
								potion = potion - 1;
							}
							else {
								player.set_hp(7000);
								cout << player.get_name() << "�͑S�񕜂����I" << endl;
								cout << player.get_name() << "�̗̑͂�" << player.get_hp() << endl;
								potion = potion - 1;
							}
							item = things.begin();	//	�C�e���[�^�ō폜
							++item;
							item = things.erase(item);
						}
						else if (number == 0){
							cout << enemy.get_name() << "��Catalina doll�Ƀ����������I�I�I" << endl;
							MeroMeroCount = 3;
							MeroMeroFlag = true;
						}
					}
				}
				if (OnceMoreFlag == true)
					continue;//�R�}���h���͂�
			}

			enemy.set_hp(enemy.get_hp() - damage);//�_���[�W�v�Z

			//�G�̐������m�F
			if (enemy.get_hp() > 0)
				cout << enemy.get_name() << "��HP�͎c��" << enemy.get_hp() << endl;
			else {
				cout << player.get_name() << "��" << enemy.get_name() << "��|�����I�I" << endl;
				FinishFlag = true;
				Winner = player.get_name();
				break;
			}
			//�G�̍U������
			damage = enemy.attack();
			cout << enemy.get_name() << "�̍U��" << endl;
			if (MeroMeroFlag == true){	//�����������
				cout << enemy.get_name() << "��Catalina doll�ɖ����Ȃ悤���c" << endl;
				MeroMeroCount -= 1;
				if (MeroMeroCount == 0)
					MeroMeroFlag = false;
			}
			else if (DefenseModeFlag != true) {
				player.set_hp(player.get_hp() - damage);
				cout << player.get_name() << "��" << damage << "�_���[�W���������" << endl;
			}
			else if (DefenseModeFlag == true) {
				player.set_hp(player.get_hp() - (damage / 2));
				cout << player.get_name() << "��" << damage / 2 << "�_���[�W���������" << endl;
			}
			

			//�����̐������m�F
			if (player.get_hp() > 0)
				cout << player.get_name() << "��HP�͎c��" << player.get_hp() << endl;
			else {
				cout << player.get_name() << "�͓|�ꂽ" << endl;
				FinishFlag = true;
				Winner = enemy.get_name();
				break;
			}
		}
	}

};

int main(){
	Game game;
	
	things.push_back("Catalina doll");	//Charactor�N���X���ł̓G���[���o��
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
		cout << "�Đ킷��H yes/no" << endl;
		cout << "decision:";
		cin >> decision;
		if (!cin.fail()){ //�G���[����
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
				cout << "���̓~�X�I" << endl;
				cout << "�Đ�Ȃ�yes/y�A�������I���ă��A���ɖ߂�Ȃ�no/n����͂��Ă�������" << endl;
				continue;
			}
		}//cin.fail�I���
		cin.clear();
		cin.seekg(0);
	}

	return 0;
}

