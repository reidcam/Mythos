#include "ActorTemplateDB.h"

/// <summary>
/// Applies the specified template to the specified actor
/// </summary>
/// <param name="actor"></param>
/// <param name="template_name"></param>
void ActorTemplate::ApplyTemplate(std::shared_ptr<Actor> actor, const std::string& template_name)
{
	// make sure the template is loaded
	LoadTemplate(actor, template_name);
}

/// <summary>
/// Reads in the specified template file if it isn't already read in
/// </summary>
/// <param name="actor"></param>
/// <param name="template_name"></param>
void ActorTemplate::LoadTemplate(std::shared_ptr<Actor> actor, const std::string& template_name)
{
	// make sure the template exists
	CheckTemplate(template_name);

	if (IsTemplateLoaded(template_name)) {
		rapidjson::Document& doc = template_documents.at(template_name); // must be a reference, doc object had std::move called on it

		//assign values
		if (doc.HasMember("name")) {
			actor->actor_name = doc["name"].GetString();
		}
		if (doc.HasMember("components")) {
			const rapidjson::Value& components = doc["components"];
			for (auto& component : components.GetObject()) { // loop through the components

				std::string rigidbody_string = "Rigidbody";
				if (component.value["type"].GetString() != rigidbody_string) {
					ComponentDB::ComponentExists(component.value["type"].GetString());
					ComponentDB::InitComponent(component.value["type"].GetString());

					actor->components[component.name.GetString()].type = component.value["type"].GetString(); // set the type
					actor->components[component.name.GetString()].componentRef = std::make_shared<luabridge::LuaRef>(luabridge::newTable(StateDB::lua_state));

					// make a deep copy of the component
					ComponentDB::EstablishInheritance(*actor->components[component.name.GetString()].componentRef,
						*ComponentDB::component_tables.at(component.value["type"].GetString()));
				}
				else { // component is a rigidbody
					actor->components[component.name.GetString()] = ComponentDB::CreateRigidbody();
				}


				// add other member variables
				luabridge::LuaRef table = (*actor->components[component.name.GetString()].componentRef);

				table["key"] = component.name.GetString();
				table["enabled"] = true;
				table["type"] = component.value["type"].GetString();

				for (auto& variable : component.value.GetObject()) { // loop through the variables on each component
					if (variable.value.IsString())
						table[variable.name.GetString()] = variable.value.GetString();
					else if (variable.value.IsFloat())
						table[variable.name.GetString()] = variable.value.GetFloat();
					else if (variable.value.IsInt())
						table[variable.name.GetString()] = variable.value.GetInt();
					else if (variable.value.IsBool())
						table[variable.name.GetString()] = variable.value.GetBool();
				}

				actor->InjectConvenienceReference(actor->components[component.name.GetString()].componentRef);

				// set life cylce function variables
				ComponentDB::SetComponentLifecylceVariables(actor->components[component.name.GetString()]);
			}
		}
	}
}

/// <summary>
/// Checks to see if template_name is a valid template file
/// </summary>
/// <param name="template_name"></param>
/// <returns></returns>
bool ActorTemplate::CheckTemplate(const std::string& template_name)
{
	if (!std::filesystem::exists("resources/actor_templates/" + template_name + ".template")) {
		std::cout << "error: template " + template_name + " is missing";
		exit(0);
	}
	return true;
}

/// <summary>
/// Checks to see if a template is already loaded into memory and loads it if it isn't
/// </summary>
/// <param name="template_name"></param>
/// <returns></returns>
bool ActorTemplate::IsTemplateLoaded(const std::string& template_name)
{
	if (template_documents.find(template_name) == template_documents.end()) { // template not found, load it
		std::string file_path = "resources/actor_templates/" + template_name + ".template";
		rapidjson::Document doc;
		EngineUtils::ReadJsonFile(file_path, doc);
		template_documents.insert(std::pair<std::string, rapidjson::Document>(template_name, std::move(doc)));
	}

	return true;
}