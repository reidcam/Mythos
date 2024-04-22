#ifndef ACTORTEMPLATEDB_H
#define ACTORTEMPLATEDB_H

#include <vector>
#include <filesystem>
#include <iostream>
#include <map>
#include "Actor.h"
#include "EngineUtilsh.h"
#include "ImageDB.h"
#include "Renderer.h"

class ActorTemplate
{
public:
	static void ApplyTemplate(std::shared_ptr<Actor> actor, const std::string& template_name);
	static void LoadTemplate(std::shared_ptr<Actor> actor, const std::string& template_name);

private:
	static bool CheckTemplate(const std::string& template_name);
	static bool IsTemplateLoaded(const std::string& template_name);
	static inline std::map<std::string, rapidjson::Document> template_documents;
};
#endif