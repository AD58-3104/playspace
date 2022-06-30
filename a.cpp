/*
CITBrainsのメンバーを管理するプログラム
管理する情報は、メンバーの学籍番号、名前、在籍してるかどうか、学年
*/

#include <iostream>
#include <bitset>
#include <string>

class menber_info
{
private:
	std::string student_number = "19C1012";
	std::string name = "inoue satoshi";

public:
	bool is_participated;
	int grade;
	void printData()
	{
		std::cout << menbers[i].student_number << menbers[i].name << std::endl;
		return;
	}
};

void printMenberData(menber_info info)
{
	std::cout << menbers[i].student_number << menbers[i].name << std::endl;
	return;
}

int main()
{
	menber_info menbers[30];
	menbers[0].grade = 4;			   // 4年
	menbers[0].is_participated = true; //参加してる

	//メンバー全員の情報をcoutとかで出力したい！
	for (size_t i = 0; i < count; i++)
	{
		menbers[i].printData();
		printMenberData(menbers[8]);
	}

	return 0;
}
