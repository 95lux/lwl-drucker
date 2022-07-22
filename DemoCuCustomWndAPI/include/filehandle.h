class filehandle {
private:
	const char* logfile_path;

public:
	filehandle(const char*);
	void write_line(std::string, std::fstream&);
	std::string get_filename();
	void init_file(std::fstream& file);
};

