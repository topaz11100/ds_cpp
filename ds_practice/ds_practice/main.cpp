#include "base.h"
#include "heap.h"

void input_to_vector(istream& in, vector<string>& v)
{
	v.clear();
	string temp;
	getline(in, temp);
	stringstream ss{ temp };

	if (temp.empty())
	{
		return; // 아무 것도 하지 않고 바로 리턴
	}

	for (string word; ss >> word;)
	{
		v.push_back(word);
	}
}

void p1()
{

}

void p2()
{

}

int main()
{	
	p1();
	p2();

	return 0;
}

/*
필기
07-12 단원
스택의 개념
덱을 뭘 써서 구현했는가?
힙의 개념적 내용
이진트리의 종류
재귀의 장단점

실기
우선순위 큐 구현
kd-tree 구현 예제

실기 문제는 코드 빈칸 채우기 식으로 출제
*/
