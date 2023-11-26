#include "Asset/SpineAtlas.h"

// загрузка атласа, store::Load и store::Free Ц функции движка, загружающие файл в пам€ть и освобождающие пам€ть соответственно.
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
// выгрузка атласа
SpineAtlas::~SpineAtlas()
{
}