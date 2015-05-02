// Uriel Naranjo
#include <iostream> 
#include <errno.h>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <map>
#include <pwd.h>
#include <grp.h>

using namespace std;

void getName(bool hidden,char *file, vector<string> &v);
void print(bool hidden, char *file); // print only non hidden files
void printL(bool hidden, char *file); // print long 
void printR(bool hidden, char *file); //print recursive
void printLR(bool hidden, char *file); //print long recursive
bool lowCase(string str1, string str2); // ignore letter case when sorting

int main(int argc, char **argv){
	if(argc < 2){
		char arg[] = ".";
		print(false, arg);
	}
	else{
		bool a, R, l = false;
		vector<char*> files, flags;
		for(int i =1; i < argc; i++){
			if(argv[i][0] == '-'){
				string str = string(argv[i]);
				if(str == "-a") 
					a = true;
				else if(str == "-l")
					l = true;
				else if(str == "-R")
					R = true;
				else if(str == "-la" || str == "-al"){
					a = true;
					l = true;
				}
				else if(str == "-aR" || str == "-Ra"){
					a = true; 
					R = true;
				}
				else if(str == "-lR" || str == "-Rl"){
					l = true;
					R = true;
				}
				else if(str == "-lRa"|| str == "Rla" || str == "-alR" || str == "-laR" || str == "-Ral" || str == "-aRl" ){
					a = true;
					l = true;
					R = true;
				}
				else{
					cerr << "ls: invalid option '" << argv[i] << "'" << endl;
				}
			}
			else 
				files.push_back(argv[i]);
		}

		if(files.size() == 0){
			char arg[] = ".";
			files.push_back(arg);
		}
		for(size_t i = 0; i < files.size(); i++){
			if(files.size() > 1){
				cout << files.at(i) << ((files.at(i)[0] != '/') ? "/:" : ":");
				cout << endl;
			}
			if(!a && !l && !R)
				print(a, files.at(i));
			else if(a && !l && !R)
				print(a, files.at(i));
			else if(R && !l)
				printR(a, files.at(i));
			else if(l && !R)
				printL(a, files.at(i));	
			else if(l && R)
				printLR(a, files.at(i));
			if(i != files.size() - 1)
				cout << endl;
		}
	}
	return 0;
}
void print(bool hidden, char *file){
	vector<string> v;
	getName(hidden, file, v);
	if(v.empty())
		return;
	unsigned int col = v.at(0).length();
	for(size_t i = 1; i < v.size(); i++){
		if(v.at(i).length() > col)
			col = v.at(i).length();
	}

	for(size_t i = 0; i < v.size() ; i++){
		if((i+1) % 6 == 0) 
			cout << endl;
		cout << setw(col) << left << v.at(i) << "  ";
	}
	cout << endl;
}

void getName(bool hidden, char *file, vector<string> &v){
	struct stat info;
	if(-1 == stat(file, &info)){
		perror("Error stat failed");
		exit(1);
	}
	if(!(info.st_mode & S_IFDIR)){
		if((string(file).at(0) != '.') && (!hidden))
			v.push_back(file);
		else if(hidden){
			v.push_back(file);
		}
		return;
	}
	auto dir = opendir(file);
	if(dir == NULL){
		perror("opendir failed.");
		exit(1);
	}
	while(auto name = readdir(dir)){
		if(name == NULL){
			perror("readdir failed");
			exit(1);
		}
		if(hidden){
			v.push_back(name->d_name);
		}
		else if((!hidden) && (string(name->d_name).at(0) != '.')){
			v.push_back(name->d_name);
		}
	}
	sort(v.begin(), v.end(),lowCase);

	if(-1 == closedir(dir)){
		perror("closedir failed.");
		exit(1);
	}
}

void printL(bool hidden, char *file){
	struct stat finfo;
	string nfile = string(file);
	nfile.append("/");
	map<int, string> month; // used to conv int month to str month
	month[1] = "Jan";
	month[2] = "Feb";
	month[3] = "Mar";
	month[4] = "Apr";
	month[5] = "May";
	month[6] = "Jun";
	month[7] = "Jul";
	month[8] = "Aug";
	month[9] = "Sep";
	month[10] = "Oct";
	month[11] = "Nov";
	month[12] = "Dec";
	if(-1 == stat(file, &finfo)){
		perror("Error stat failed");
		exit(1);
	}
	if(S_IFDIR & finfo.st_mode){
		auto dir = opendir(file);
		if(dir == NULL){
			perror("opendir failed.");
			exit(1);
		}
		vector<char*> files;
		
		while(auto dirinfo = (readdir(dir))){
			if(dirinfo == NULL) {
				perror("readdir failed.");
				exit(1);
			}
			//if(string(dirinfo->d_name) == "ls"){}
			if (!hidden && string(dirinfo->d_name).at(0) != '.'){
				files.push_back(dirinfo->d_name);
			}
			else if(hidden){
				files.push_back(dirinfo->d_name);
			}
		}
		sort(files.begin(), files.end(), lowCase);
		for(size_t i = 0; i < files.size(); i++){
			struct stat info;
			string temp = string(files[i]);
			string tmp = nfile;
			tmp.append(temp);
			if(-1 == lstat((char*)tmp.c_str() , &info)){
				perror("Error with stat");
				exit(1);
			}
			cout << ((S_IFDIR & info.st_mode) ? "d"  : "-"); 
			cout << ((info.st_mode & S_IRUSR) ? "r"  : "-");
			cout << ((info.st_mode & S_IWUSR) ? "w"  : "-");
			cout << ((info.st_mode & S_IXUSR) ? "x"  : "-");
			cout << ((info.st_mode & S_IRGRP) ? "r"  : "-");
			cout << ((info.st_mode & S_IWGRP) ? "w"  : "-");
			cout << ((info.st_mode & S_IXGRP) ? "x"  : "-");
			cout << ((info.st_mode & S_IROTH) ? "r"  : "-");
			cout << ((info.st_mode & S_IWOTH) ? "w"  : "-");
			cout << ((info.st_mode & S_IXOTH) ? "x"  : "-");
			cout << " " << info.st_nlink;
			auto id = getpwuid(info.st_uid);
			if(id == NULL){
				perror("getpwuid failed.");
				exit(1);
			}
			auto grid = getgrgid(finfo.st_gid);
			if(grid == NULL){
				perror("getgrgid failed");
				exit(1);
			}
			auto time = localtime(&(info.st_mtime));
			if(time == NULL){
				perror("localtime failed.");
				exit(1);
			}
			cout << " " << id->pw_name;
			cout << " " << grid->gr_name;
			cout << " " << setfill(' ') << setw(5) << info.st_size;
			cout << " " << month[time->tm_mon + 1] << ' '
				<< setfill(' ') << setw(2) << time->tm_mday;
	    	cout << ' ' << setfill('0') << setw(2) << time->tm_hour << ":"
				 << setfill('0') << setw(2) << time->tm_min; 
			cout << " " << files.at(i);
			cout << endl;
		}
		if(-1 == closedir(dir)){
			perror("closedir failed.");
			exit(1);
		}

	}
	else{
		cout << ((S_IFDIR & finfo.st_mode) ? "d"  : "-");
		cout << ((finfo.st_mode & S_IRUSR) ? "r"  : "-");
		cout << ((finfo.st_mode & S_IWUSR) ? "w"  : "-");
		cout << ((finfo.st_mode & S_IXUSR) ? "x"  : "-");
		cout << ((finfo.st_mode & S_IRGRP) ? "r"  : "-");
		cout << ((finfo.st_mode & S_IWGRP) ? "w"  : "-");
		cout << ((finfo.st_mode & S_IXGRP) ? "x"  : "-");
		cout << ((finfo.st_mode & S_IROTH) ? "r"  : "-");
		cout << ((finfo.st_mode & S_IWOTH) ? "w"  : "-");
		cout << ((finfo.st_mode & S_IXOTH) ? "x"  : "-");
		cout << " " << finfo.st_nlink;
		auto id = getpwuid(finfo.st_uid);
		if(id == NULL){
			perror("getpwuid failed.");
			exit(1);
		}
		auto grid = getgrgid(finfo.st_gid);
		if(grid == NULL){
			perror("getgrgid failed");
			exit(1);
		}
		auto time = localtime(&(finfo.st_mtime));
		if(time == NULL){
			perror("localtime failed");
			exit(1);
		}
		cout << " " << id->pw_name;
		cout << " " << grid->gr_name;
		cout << " " << finfo.st_size;
		cout << " " << month[time->tm_mon + 1] << ' '
			<< setfill(' ') << setw(2)<< time->tm_mday << ' ';
		int hour = time->tm_hour;
		cout << hour;
		cout << ":";
		cout << setfill('0') << setw(2) << localtime(&(finfo.st_mtime))->tm_min; 
		cout << " " << file;
		cout << endl;
 	}
}

void printR(bool hidden, char *file){
	vector<string> dir;
	struct stat finfo;
	if(-1 == lstat(file, &finfo)){
		perror("lstat failed");
		exit(1);
	}
	string filename = string(file);
	if(finfo.st_mode & S_IFDIR){
		if(filename.at(filename.size()-1) != '/')
				filename.append("/");
		auto direc = opendir(file);
		if(direc == NULL){
				perror("opendir failed");
				exit(1);
		}
		string temp;
		while(auto dinfo = (readdir(direc))){
			if(dinfo == NULL){
				perror("readdir failed");
				exit(1);
			}
			struct stat info;
			string curr = filename;
			string cfile = curr.append(string(dinfo->d_name));
			if(-1 == stat((char*)cfile.c_str(), &info)){
				perror("stat failed");
				exit(1);
			}
			if(/*(string(dinfo->d_name) != "ls") &&*/ (S_IFDIR & info.st_mode)){
				if((!(hidden) && (string(dinfo->d_name).at(0) == '.' ))){
				//	string tmp = filename;
				//	temp = tmp.append(dinfo->d_name);
				//	dir.push_back(temp);
				}
				else if((hidden) && !((string(dinfo->d_name) == ".") || (string(dinfo->d_name) == ".."))){ 
					string tmp = filename;
					temp = tmp.append(dinfo->d_name);
					dir.push_back(temp);
				}
				else if(!(hidden) && (string(dinfo->d_name).at(0) != '.')){
					string tmp = filename;
					temp = tmp.append(dinfo->d_name);
					dir.push_back(temp);
				}
			}
		}
		if(-1 == closedir(direc)){
			perror("closedir failed");
			exit(1);
		}
		cout << filename << ':' << endl;
	}
	print(hidden, file);
	if(dir.size() == 0) 
		return;


	sort(dir.begin(), dir.end(), lowCase);
	for(size_t i = 0; i < dir.size(); i++){
		cout << endl;
		printR(hidden, (char*)dir.at(i).c_str());
	}
}

bool lowCase(string str1, string str2){
	return (tolower(str1.at(0)) < tolower(str2.at(0)));
}

void printLR(bool hidden, char *file){
	vector<string> dir;
	string filename = string(file);
	if(filename.at(filename.size()-1) != '/')
		filename.append("/");
	struct stat finfo;
	if(-1 == stat(file, &finfo)){
		perror("stat failed");
		exit(1);
	}
	if(finfo.st_mode & S_IFDIR){
		string filename = string(file);
		if(filename.at(filename.size()-1) != '/')
				filename.append("/");
		auto direc = opendir(file);
		if(direc == NULL){
				perror("opendir failed");
				exit(1);
		}
		string temp;
		while(auto dinfo = (readdir(direc))){
			if(dinfo == NULL){
				perror("readdir failed");
				exit(1);
			}
			struct stat info;
			string curr = filename;
			string cfile = curr.append(string(dinfo->d_name));
			if(-1 ==stat((char*)cfile.c_str(), &info)){
				perror("stat failed");
				exit(1);
			}
			if((S_IFDIR & info.st_mode)){
				if((!(hidden) && (string(dinfo->d_name).at(0) == '.' ))){
				//	string tmp = filename;
				//	temp = tmp.append(dinfo->d_name);
				//	dir.push_back(temp);
				}
				else if((hidden) && !((string(dinfo->d_name) == ".") || (string(dinfo->d_name) == ".."))){ 
					string tmp = filename;
					temp = tmp.append(dinfo->d_name);
					dir.push_back(temp);
				}
				else if(!(hidden) && (string(dinfo->d_name).at(0) != '.')){
					string tmp = filename;
					temp = tmp.append(dinfo->d_name);
					dir.push_back(temp);
				}
			}
		}
		if(-1 == closedir(direc)){
			perror("closedir failed");
		}
		cout << filename << ':' << endl;
	}

	printL(hidden, file);
	if(dir.size() == 0) 
		return;

	sort(dir.begin(), dir.end());
	for(size_t i = 0; i < dir.size(); i++){
		cout << endl;
		printLR(hidden, (char*)dir.at(i).c_str());
	}
}

