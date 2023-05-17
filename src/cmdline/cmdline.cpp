#include "cmdline.h"

#include <iomanip>
#include <string.h>

using namespace lmc;

#define YELLOW  "\033[33m"
#define RESET   "\033[0m"
#define RED     "\033[31m"

void CmdLine::parse(bool noParam, int argc, char *argv[])
{
    cmd = argv[0];
    add("-h", "--help", "print help message");

    if (!noParam && argc <= 1)
        showHelp();

    std::shared_ptr<ParamBase> pB = nullptr;
    bool bSearch = false;
    try
    {
        for (int i = 1; i < argc; i++)
        {
            bSearch = false;
            if (!strncmp(argv[i], "--", 2) || !strncmp(argv[i], "-", 1))
            {
                if ((!strncmp(argv[i], "--help", 6) && strlen(argv[i]) == 6) ||
                    (!strncmp(argv[i], "-h", 2) && strlen(argv[i]) == 2))
                    showHelp();

                for (auto &l : paramTable)
                {
                    if (l->getName() == argv[i] || 
                        l->getShortName() == argv[i])
                    {
                        bSearch = true;
                        l->setEnable(true);
                        pB = l;
                        break;
                    }
                }

                if (!bSearch)
                {
                    CmdLineError err;
                    err << "    " << argv[i];
                    throw err;
                }
            }
            else
            {
                if (!pB) 
                    showHelp();

                pB->set(argv[i]);
            }
        }
    }
    catch(const std::exception &e)
    {
        std::cout << RED << "options is invaild:" << RESET << std::endl;
        std::cout << YELLOW << e.what() << RESET << std::endl;
        std::cout << std::endl;
        showHelp();
    }

    paramCheck();
}

void CmdLine::paramCheck()
{
    std::string name = "";
    std::string shortName = "";
    try
    {
        std::set<std::string> enableSet;
        for (auto &l : paramTable)
        {
            if (l->getEnable())
            {
                enableSet.emplace(l->getName());
                enableSet.emplace(l->getShortName());
            }
        }

        for (auto &l : paramTable)
        {
            if (l->getEnable())
            {
                name = l->getName();
                shortName = l->getShortName();
                l->searchDeps(enableSet);
            } 
        }
    }
    catch(const std::exception &e)
    {
        std::cout << RED  << "options error:" << RESET << std::endl;
        std::string str = std::string("option ( ") + shortName + " , " + 
                          name + " ) depends option " + e.what();
        std::cout << YELLOW << str << RESET << std::endl;
        std::cout << std::endl;
        showHelp();
    }
}

void CmdLine::showHelp()
{
    std::cout << "usage: " << cmd << " [options] ..." << std::endl;
    std::cout << "options:" << std::endl;
    for (auto &l : paramTable)
    {
        std::cout << "    "   << std::left << std::setfill(' ')
                  << std::setw(10) << l->getShortName()<< ", "
                  << std::left << std::setfill(' ') << std::setw(25)
                  << l->getName() << l->getDescription() << std::endl;

        std::string depStr = l->getDepsStr();
        if (!depStr.empty())
            std::cout <<std::left << std::setfill(' ')
                      << std::setw(41) << " " << "option depends "
                      << depStr << std::endl;

        std::string rangeStr = l->getRangeStr();
        if (!rangeStr.empty())
            std::cout <<std::left << std::setfill(' ') 
                      << std::setw(41) << " " << "parameter range is "
                      << rangeStr << std::endl;
    }

    exit(0);
}

std::string ParamBase::getDescription()
{
    return mDescribtion;
}

std::string ParamBase::getName()
{
    return mName;
}

std::string ParamBase::getShortName()
{
    return mShortName;
}

void ParamBase::setEnable(bool enable)
{
    mEnable = enable;
}

 bool ParamBase::getEnable()
 {
    return mEnable;
 }

ParamBase::ParamBase(const std::string &name_,
                     const std::string &shortName_,
                     const std::string &describtion_) :
                     mName(name_),
                     mShortName(shortName_),
                     mDescribtion(describtion_),
                     mEnable(false) {}