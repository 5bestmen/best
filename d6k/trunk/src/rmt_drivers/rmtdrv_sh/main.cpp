#include "dialog.h"
#include <QApplication>
#include <iostream>  
#include <boost/program_options.hpp>

int main(int argc, char *argv[])
{
	//using namespace boost::program_options; 
	std::string szDllName ,szProjectName;
	int nOccNo = 0;


	try
	{
		boost::program_options::options_description desc("Allowed options");

		desc.add_options()
			("help,h", "produce help message")
			("dll,d", boost::program_options::value<std::string>(), "set dll name")
			("project,p", boost::program_options::value<std::string>(), "set project name")
			("occno,o", boost::program_options::value<int>(), "set app occno");

		boost::program_options::variables_map vm;
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
		boost::program_options::notify(vm);

		if (vm.count("help"))
		{
			std::cout << desc << "\n";
		}

		if (vm.count("dll"))
		{
			szDllName = vm["dll"].as<std::string>();
		}
		if (vm.count("project"))
		{
			szProjectName = vm["project"].as<std::string>();
		}
		if (vm.count("occno"))
		{
			nOccNo = vm["occno"].as<int>();
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << "error: " << e.what() << "\n";
		return 1;
	}
	catch (...)
	{
		std::cerr << "Exception of unknown type!  \n";
	}


    QApplication a(argc, argv);

	// 获取命令行参数
	QStringList arguments = QCoreApplication::arguments();


	CShDialog w;
    w.show();

    return a.exec();
}
