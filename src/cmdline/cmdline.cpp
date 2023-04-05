#include "cmdline.h"

#include <ios>
#include <iomanip>
#include <string.h>

using namespace lmc;

void CmdLine::parse(int argc, char *argv[])
{
    cmd = argv[0];
    paramTable.emplace_back(std::make_shared<ParamNone>("--help", 
                                                        "-h",
                                                        "print help message"));
    std::shared_ptr<ParamBase> pB = nullptr;
    bool bSearch = false;
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

            int value = ::atoi(argv[i]);
            if (value || (!value && !strcmp(argv[i], "0")))
                pB->set(value);
            else
                pB->set(argv[i]);

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

bool ParamNone::hasParam()
{
    return false;
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
                     mEnable(false)
{}