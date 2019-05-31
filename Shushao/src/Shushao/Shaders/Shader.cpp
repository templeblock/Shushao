#include "sepch.h"

#include <glad/glad.h>

#include "Shader.h"
#include "Shushao/Debug.h"
#include "Shushao/Light.h"
#include "Shushao/utility.h"

namespace se {

	//{ #region uniform

	void Uniform::SetFloat(GLfloat& value) {
		glUniform1f(location, value);
	}

	void Uniform::SetInteger(GLint& value) {
		glUniform1i(location, value);
	}

	void Uniform::SetTextureIndex(GLint& value) {
		glUniform1i(location, (GLint)value);
		texture = value;
	}

	void Uniform::SetMatrix(GLfloat* value) {
		glUniformMatrix4fv(location, 1, GL_FALSE, value);
	}

	void Uniform::SetVector(glm::vec4& value) {
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Uniform::SetVector(glm::vec3& value) {
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Uniform::SetVector(glm::vec2& value) {
		glUniform2f(location, value.x, value.y);
	}

	void Uniform::SetColor(Color& value) {
		glUniform4f(location, value.r, value.g, value.b, value.a);
	}

	//}

	//{ #region uniform setter

	void Shader::SetFloat(std::string var_, GLfloat value) {
		auto it = uniforms.find(var_);
		if (it != uniforms.end()) {
			it->second.SetFloat(value);
		}
		else {
			glUniform1f(glGetUniformLocation(programID, var_.c_str()), (GLfloat)value);
		}
	}

	void Shader::SetInteger(std::string var_, GLint value) {
		auto it = uniforms.find(var_);
		if (it != uniforms.end()) {
			it->second.SetInteger(value);
		}
		else {
			glUniform1i(glGetUniformLocation(programID, var_.c_str()), (GLint)value);
		}
	}

	void Shader::SetTextureIndex(std::string var_, GLint value) {
		auto it = uniforms.find(var_);
		if (it != uniforms.end()) {
			it->second.SetTextureIndex(value);
		}
		else {
			glUniform1i(glGetUniformLocation(programID, var_.c_str()), (GLint)value);
		}
	}

	void Shader::SetLight(std::string var_, UniformLight& value) {
		glUniform3f(glGetUniformLocation(programID, (var_ + ".position").c_str()), value.position[0], value.position[1], value.position[2]);
		glUniform3f(glGetUniformLocation(programID, (var_ + ".direction").c_str()), value.direction[0], value.direction[1], value.direction[2]);
		glUniform3f(glGetUniformLocation(programID, (var_ + ".ambient").c_str()), value.ambient[0], value.ambient[1], value.ambient[2]);
		glUniform3f(glGetUniformLocation(programID, (var_ + ".diffuse").c_str()), value.diffuse[0], value.diffuse[1], value.diffuse[2]);
		glUniform3f(glGetUniformLocation(programID, (var_ + ".specular").c_str()), value.specular[0], value.specular[1], value.specular[2]);
		glUniform1f(glGetUniformLocation(programID, (var_ + ".linear").c_str()), value.linear);
		glUniform1f(glGetUniformLocation(programID, (var_ + ".quadratic").c_str()), value.quadratic);
		glUniform1f(glGetUniformLocation(programID, (var_ + ".cutoff").c_str()), value.cutoff);
	}

	void Shader::SetMatrix(std::string var_, GLfloat* value) {
		auto it = uniforms.find(var_);
		if (it != uniforms.end()) {
			it->second.SetMatrix(value);
		}
		else {
			glUniformMatrix4fv(glGetUniformLocation(programID, var_.c_str()), 1, GL_FALSE, (GLfloat*)value);
		}
	}

	void Shader::SetVector(std::string var_, glm::vec3 value) {
		auto it = uniforms.find(var_);
		if (it != uniforms.end()) {
			it->second.SetVector(value);
		}
		else {
			glUniform3f(glGetUniformLocation(programID, var_.c_str()), value.x, value.y, value.z);
		}
	}

	void Shader::SetVector(std::string var_, glm::vec4 value) {
		auto it = uniforms.find(var_);
		if (it != uniforms.end()) {
			it->second.SetVector(value);
		}
		else {
			glUniform4f(glGetUniformLocation(programID, var_.c_str()), value.x, value.y, value.z, value.w);
		}
	}

	void Shader::SetVector(std::string var_, glm::vec2 value) {
		auto it = uniforms.find(var_);
		if (it != uniforms.end()) {
			it->second.SetVector(value);
		}
		else {
			glUniform2f(glGetUniformLocation(programID, var_.c_str()), value.x, value.y);
		}
	}

	void Shader::SetColor(std::string var_, Color value) {
		auto it = uniforms.find(var_);
		if (it != uniforms.end()) {
			it->second.SetColor(value);
		}
		else {
			glUniform4f(glGetUniformLocation(programID, var_.c_str()), value.r, value.g, value.b, value.a);
		}
	}

	GLenum Shader::GetTextureIndex(std::string var_) {
		auto it = uniforms.find(var_);
		if (it != uniforms.end()) return it->second.texture;
		return 0;
	}

	void Shader::SetMVP(GLfloat* value) {
		SetMatrix("MVP", value);
	}

	void Shader::SetMV(GLfloat* value) {
		SetMatrix("MV", value);
	}

	void Shader::SetVP(GLfloat* value) {
		SetMatrix("VP", value);
	}

	void Shader::SetM(GLfloat* value) {
		SetMatrix("M", value);
	}

	void Shader::SetP(GLfloat* value) {
		SetMatrix("P", value);
	}

	void Shader::SetV(GLfloat* value) {
		SetMatrix("V", value);
	}

	void Shader::SetRenderColor(Color value) {
		SetColor("render_color", value);
	}

	void Shader::Enable(std::string var_) {
		SetInteger("enabled_" + var_, 1);
	}

	void Shader::Disable(std::string var_) {
		SetInteger("enabled_" + var_, 0);
	}

	//}

	Shader::Shader() {
		name = "Shader";
	}

	Shader::Shader(std::string filename, std::string n) {
		loadWithName(filename, n);
	}

	Shader::Shader(std::string filename) {
		loadWithName(filename, util::basename(filename));
	}

	bool Shader::loadWithName(std::string filename, std::string n) {
		if (Load(filename)) {
			name = n;
			Init();
			return true;
		}
		return false;
	}

	Shader::~Shader() {
		Debug::Log << "Destructor di Shader " << name << std::endl;

		uniforms.clear();
		Leave();

		if (VertexShaderID > 0) glDeleteShader(VertexShaderID);
		if (FragmentShaderID > 0) glDeleteShader(FragmentShaderID);
		if (GeometryShaderID > 0) glDeleteShader(GeometryShaderID);
		if (programID > 0) glDeleteProgram(programID);
	}

	bool Shader::Init() {
		if (programID > 0) {
			Leave();
			glDeleteProgram(programID);
		}

		if (!Shader::compile()) {
			Debug::Log(ERROR) << "Error Compiling Shader" << std::endl;
			return false;
		}

		if (!Shader::link()) {
			Debug::Log(ERROR) << "Error Linking Shader" << std::endl;
			return false;
		}

		// base uniforms
		AddShaderUniform("Model View Projection", "MVP", Uniform::Type::MATRIX, ShaderLocation::LOCATION_MVP);
		AddShaderUniform("Render Color", "render_color", Uniform::Type::COLOR, ShaderLocation::LOCATION_RENDER_COLOR);

		return true;
	}

	void Shader::AddUniform(std::string name_, std::string var_, Uniform::Type type_, GLuint location_) {
		uniforms.insert(std::pair<std::string, Uniform>(var_, { name_, var_, type_, location_, false }));
	}

	void Shader::AddUniform(std::string var_, Uniform::Type type_, GLuint location_) {
		uniforms.insert(std::pair<std::string, Uniform>(var_, { var_, var_, type_, location_, false }));
	}

	void Shader::AddShaderUniform(std::string var_, Uniform::Type type_, GLuint location_) {
		uniforms.insert(std::pair<std::string, Uniform>(var_, { var_, var_, type_, location_, true }));
	}

	void Shader::AddShaderUniform(std::string name_, std::string var_, Uniform::Type type_, GLuint location_) {
		uniforms.insert(std::pair<std::string, Uniform>(var_, { name_, var_, type_, location_, true }));
	}

	GLuint Shader::GetProgram() {
		if (programID > 0)
			return programID;
		else if (Init())
			return programID;

		return 0;
	}

	void Shader::awake() {
		GLuint programID = GetProgram();
		if (!programID) return;

		Use();
		Awake();

		// setting uniform locations where not set
		for (auto& it : uniforms) {
			if (!it.second.location) {
				it.second.location = glGetUniformLocation(programID, (const GLchar*)it.first.c_str());
			}
		}

		Leave();
	}

	void Shader::update() {
		Update();
	}

	void Shader::exit() {
		Exit();
		Leave();
	}

	void Shader::Awake() {}
	void Shader::Update() {}
	void Shader::Exit() {}

	void Shader::Use() {
		if (!programID) return;
		glUseProgram(programID);
		inUse = true;
	}

	void Shader::Leave() {
		if (!programID) return;
		glUseProgram(0);
		inUse = false;
	}

	//{ #region compile

	bool Shader::compile() {
		if (FragmentShaderCode != "") {
			// Compile Vertex Shader
			VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			char const* VertexSourcePointer = VertexShaderCode.c_str();
			glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
			glCompileShader(VertexShaderID);
			// Check Vertex Shader
			if (!shaderCompilationLog(VertexShaderID))
				return false;
		}

		if (FragmentShaderCode != "") {
			// Compile Fragment Shader
			FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
			char const* FragmentSourcePointer = FragmentShaderCode.c_str();
			glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
			glCompileShader(FragmentShaderID);
			// Check Vertex Shader
			if (!shaderCompilationLog(FragmentShaderID))
				return false;
		}

		if (GeometryShaderCode != "") {
			// Compile Geometry Shader
			GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
			char const* GeometrySourcePointer = GeometryShaderCode.c_str();
			glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer, NULL);
			glCompileShader(GeometryShaderID);
			// Check Geometry Shader
			if (!shaderCompilationLog(GeometryShaderID))
				return false;
		}

		return true;
	}

	bool Shader::link() {
		// Link the program
		programID = glCreateProgram();
		glAttachShader(programID, VertexShaderID);
		glAttachShader(programID, FragmentShaderID);
		if (GeometryShaderID > 0) glAttachShader(programID, GeometryShaderID);
		glLinkProgram(programID);

		// Check the program
		if (!programCompilationLog(programID))
			return false;

		// free resources
		if (VertexShaderID > 0) glDeleteShader(VertexShaderID);
		if (FragmentShaderID > 0) glDeleteShader(FragmentShaderID);
		if (GeometryShaderID > 0) glDeleteShader(GeometryShaderID);

		return true;
	}

	void Shader::LoadFromString(std::string vsc, std::string fsc, std::string gsc) {
		VertexShaderCode = vsc;
		FragmentShaderCode = fsc;
		GeometryShaderCode = gsc;
	}

	bool Shader::Load(std::string shaderfile) {
		// TODO: Load Geometry Shader code

		std::string vert = shaderfile + ".vert";
		std::string frag = shaderfile + ".frag";

		// Read the Vertex Shader code from the file
		std::ifstream VertexShaderStream(vert.c_str(), std::ifstream::in);
		if (VertexShaderStream.is_open()) {
			std::string Line = "";
			while (std::getline(VertexShaderStream, Line))
				VertexShaderCode += "\n" + Line;
			VertexShaderStream.close();

		}
		else {
			Debug::Log(ERROR, SOURCE) << "Impossibile aprire il file " << vert << std::endl;
		}

		// Read the Fragment Shader code from the file
		std::ifstream FragmentShaderStream(frag.c_str(), std::ifstream::in);
		if (FragmentShaderStream.is_open()) {
			std::string Line = "";
			while (std::getline(FragmentShaderStream, Line))
				FragmentShaderCode += "\n" + Line;
			FragmentShaderStream.close();
		}
		else {
			Debug::Log(ERROR, SOURCE) << "Impossibile aprire il file " << frag << std::endl;
		}

		return true;
	}

	bool Shader::shaderCompilationLog(const GLuint& shader) {
		GLint status;
		std::string script;
		if (shader == VertexShaderID)
			script = "Vertex";
		else if (shader == FragmentShaderID)
			script = "Fragment";
		else if (shader == GeometryShaderID)
			script = "Geometry";

		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			if (Debug::enabled) Debug::Log(ERROR) << name << " " << script << " script Compilation failed:\n"
				<< infoLog << std::endl;
			return false;
		}
		return true;
	}

	bool Shader::programCompilationLog(const GLuint& program) {
		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			GLint infoLogLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar* infoLog = new GLchar[infoLogLength];
			glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
			if (Debug::enabled) Debug::Log(ERROR) << "Program " << name << " Compilation failed:\n"
				<< infoLog << std::endl;
			delete[] infoLog;
			return false;
		}
		return true;
	}

	//}

}  // namespace se
