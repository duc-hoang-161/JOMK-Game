
#include "Helper\MyBodyParser.h"

MyBodyParser* MyBodyParser::getInstance()
{
	static MyBodyParser* sg_ptr = nullptr;
	if (nullptr == sg_ptr)
	{
		sg_ptr = new MyBodyParser;
	}
	return sg_ptr;
}

bool MyBodyParser::parseDoc(unsigned char *buffer, long length)
{
	bool result = false;
	std::string js((const char*)buffer, length);
	doc.Parse<0>(js.c_str());
	if (!doc.HasParseError())
	{
		result = true;
	}
	return result;
}

bool MyBodyParser::parseConfig(unsigned char *buffer, long length)
{
	bool result = false;
	std::string js((const char*)buffer, length);
	config.Parse<0>(js.c_str());
	if (!config.HasParseError())
	{
		result = true;

		anchor.x = config["Anchor"]["x"].GetFloat();
		anchor.y = config["Anchor"]["y"].GetFloat();		
	}

	return result;
}

void MyBodyParser::clearCache()
{
	doc.SetNull();
}

bool MyBodyParser::parseJsonFile(const std::string& pFile, const std::string &config)
{
	auto content = FileUtils::getInstance()->getDataFromFile(pFile);
	bool result = parseDoc(content.getBytes(), content.getSize());

	content = FileUtils::getInstance()->getDataFromFile(config);
	result = parseConfig(content.getBytes(), content.getSize());

	return result;
}

std::pair<FixtureDef*, FixtureDef*>
	MyBodyParser::bodyFormJson(const std::string& name)
{
	std::pair<FixtureDef*, FixtureDef*> fixtureDef;
	fixtureDef.first = fixtureDef.second = nullptr;

	if (doc.FindMember(name.c_str()) != doc.MemberEnd()) {
		rapidjson::Value &bd = doc[name.c_str()];
		float width = bd["size"]["width"].GetFloat();
		float height = bd["size"]["height"].GetFloat();
		int offX = -anchor.x * width;
		int offY = -anchor.y * height;

		rapidjson::Value &all = bd["polygons"];

		auto currentFirst = &fixtureDef.first;
		auto currentSecond = &fixtureDef.second;

		for (rapidjson::Value::ConstMemberIterator it = all.MemberBegin();
			it != all.MemberEnd(); ++it) {

			std::string tagName = it->name.GetString();
			int tag = config["Tag"][tagName.c_str()].GetInt();

			rapidjson::Value &polygons = all[tagName.c_str()];
			for (int i = 0; i < polygons.Size(); ++i)
			{
				*currentFirst = new FixtureDef();
				*currentSecond = new FixtureDef();

				int pcount = polygons[i].Size();
				b2Vec2* normal = new b2Vec2[pcount];
				b2Vec2* flippedX = new b2Vec2[pcount];
				for (int pi = 0; pi<pcount; ++pi)
				{
					normal[pi].x = offX + polygons[i][pcount - 1 - pi]["x"].GetDouble();
					normal[pi].y = offY + polygons[i][pcount - 1 - pi]["y"].GetDouble();
					normal[pi].x /= 32.0;
					normal[pi].y /= 32.0;

					flippedX[pi].x = offX + (width - polygons[i][pcount - 1 - pi]["x"].GetDouble());
					flippedX[pi].y = offY + polygons[i][pcount - 1 - pi]["y"].GetDouble();
					flippedX[pi].x /= 32.0;
					flippedX[pi].y /= 32.0;
				}

				b2PolygonShape *shape = new b2PolygonShape();
				shape->Set(normal, pcount);
				b2FixtureDef fixture;
				fixture.shape = shape;
				(*currentSecond)->fixture = fixture;
				(*currentSecond)->fixture.userData = (void*)tag;
				(*currentSecond)->fixture.friction = 1;
				(*currentSecond)->fixture.restitution = 0;

				shape = new b2PolygonShape();
				shape->Set(flippedX, pcount);
				fixture.shape = shape;
				(*currentFirst)->fixture = fixture;
				(*currentFirst)->fixture.userData = (void*)tag;
				(*currentFirst)->fixture.friction = 1;
				(*currentFirst)->fixture.restitution = 0;

				currentFirst = &(*currentFirst)->next;
				currentSecond = &(*currentSecond)->next;

				delete[] normal;
				delete[] flippedX;
			}
		}
	}

	return fixtureDef;
}

bool MyBodyParser::containKey(const std::string &key)
{
	return (doc.FindMember(key.c_str()) != doc.MemberEnd());
}