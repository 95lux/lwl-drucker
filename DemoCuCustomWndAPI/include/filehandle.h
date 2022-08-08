class filehandle {
private:
	const char* logfiles_path;

public:
	filehandle(const char*);
	filehandle();
	void write_line(std::string, std::fstream&);
	std::string get_filename();
	void init_file(std::fstream& file);
};

