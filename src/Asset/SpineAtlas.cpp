#include "Asset/SpineAtlas.h"

// �������� ������, store::Load � store::Free � ������� ������, ����������� ���� � ������ � ������������� ������ ��������������.
SpineAtlas::SpineAtlas(const std::string& name, Terrasu::AssetManager& mn) :
	mName(name), mAtlas(0)
{
	int length = 0;
	auto s = mn.ReadFileStr("Assets/" + name + ".atlas");
	length = s.size();
	const char* data = s.c_str();
	if (data)
	{
		mAtlas = new spine::Atlas(data, length, "", 0);
	}
}
// �������� ������
SpineAtlas::~SpineAtlas()
{
}