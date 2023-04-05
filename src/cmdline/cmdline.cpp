#include "cmdline.h"

#include <ios>
#include <iomanip>
#include <string.h>
#include <set>

using namespace lmc;

void CmdLine::parse(int argc, char *argv[])
{
    cmd = argv[0];
    paramTable.emplace_back(std::make_shared<ParamNone>("--help", 
                                                        "-h",
                                                        "print help message"));
    std::shared_ptr<ParamBase> pB = nullptr;
    bool bSearch = false;
    try
    {
        for (int i = 1; i < argc; i++)
        {
            bSearch = false;
            if (!strncmp(argv[i], "--", 2) || !strncmp(argv[i], "-", 1))
            {
                if (!strncmp(argv[i], "--help", 6) || !strncmp(argv[i], "-h", 2))
                    showHelp();

                for (auto &l : paramTable)
                {
                    if (l->getName() == argv[i] || l->getShortName() == argv[i])
                    {
                        bSearch = true;
                        l->setEnable(true);
                        pB = l;
                        break;
                    }
                }  

                if (!bSearch)
                    showHelp();
            }
            else
            {
                if (!pB) 
                    showHelp();

                pB->set(argv[i]);
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cout << "options is invaild:" << std::endl;
        std::cout << e.what() << std::endl;
        std::cout << std::endl;
        showHelp();
    }

    paramCheck();
}

void CmdLine::paramCheck()
{
    std::set<std::string> enableSet;
    for (auto &l : paramTable)
    {
        if (l->getEnable())
        {
            enableSet.emplace(l->getName());
        }
    }

    for (auto &l : paramTable)
    {
        if (l->getEnable())
        {
            auto &deps = l->getDepList();
            for (auto &d : deps)
            {
                if (enableSet.find(d) == enableSet.end())
                {
                    std::cout << "options error:" << std::endl;
                    std::string str = std::string("paramter ") +
                          l->getName() + " depends paramter " + d;
                    std::cout << str << std::endl;
                    std::cout << std::endl;
                    showHelp();
                }
            }
        } 
    }
}

void CmdLine::showHelp()
{
    std::cout << "usage: " << cmd << " [options] ..." << std::endl;
    std::cout << "options:" << std::endl;
    for (auto &l : paramTable)
    {
        std::cout << "    "   << std::left << std::setfill(' ') << std::setw(5)
                  << l->getShortName()<< ", "  <<std::left << std::setfill(' ') 
                  << std::setw(25) << l->getName() << l->getDescription() 
                  << std::endl;
        std::string str = l->getRange();
        if (!str.empty())
            std::cout <<std::left << std::setfill(' ') 
                      << std::setw(36) << " " << str << std::endl;
    }

    exit(0);
}

std::string ParamBase::getRangeStr()
{
    return getRange();
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
                     const std::string &describtion_,
                     const std::list<std::string> &dep) :
                     mName(name_),
                     mShortName(shortName_),
                     mDescribtion(describtion_),
                     mEnable(false),
                     mDep(dep)
{}

std::list<std::string> &ParamBase::getDepList()
{
    return mDep;
}