#include "cmdline.h"

#include <ios>
#include <iomanip>

ParamStringNoRange::ParamStringNoRange(const std::string &name, 
                                       const std::string &shortName, 
                                       const std::string &descryption) :
                    ParamNone(name, shortName, descryption)
{}

CmdType ParamStringNoRange::getCmdType()
{
    return CmdType::StringNoRange;
}

void ParamStringNoRange::setStr(std::string str)
{
    mStr = str;
}

std::string ParamStringNoRange::getStr()
{
    return mStr;
}

ParamHelp::ParamHelp(const std::string &name, 
                     const std::string &shortName, 
                     const std::string &descryption) :
           ParamNone(name, shortName, descryption)
{}

CmdType ParamHelp::getCmdType()
{
    return CmdType::Help;
}

ParamIntRepeat::ParamIntRepeat(const std::string &name, 
                               const std::string &shortName, 
                               const std::string &descryption,
                               int min, int max) :
                ParamNone(name, shortName, descryption),
                mMin(min), mMax(max)
{}

CmdType ParamIntRepeat::getCmdType()
{
    return CmdType::IntRepeat;
}

int ParamIntRepeat::getMin()
{
    return mMin;
}

int ParamIntRepeat::getMax()
{
    return mMax;
}

ParamBase::ParamBase() :
    enable(false)
{}

bool ParamBase::getEnable()
{
    return enable;
}

bool ParamBase::setEnable()
{
    enable = true;
}

void ParamIntRepeat::pushBack(int num)
{
    mList.push_back(num);
}

std::list<int> &ParamIntRepeat::getList()
{
    return mList;
}

ParamNone::ParamNone(const std::string &name, 
                     const std::string &shortName, 
                     const std::string &descryption) :
    mName(name), mShortName(shortName), mDescryption(descryption)
{}

CmdType ParamNone::getCmdType()
{
    return CmdType::None;
}

std::string ParamNone::getName()
{
    return mName;
}

std::string ParamNone::getShortName()
{
    return mShortName;
}

std::string ParamNone::getDescription()
{
    return mDescryption;
}

bool CmdLine::get(std::string name)
{
    for (auto &l : mParamList)
    {
        if (l->getName() == name)
        {
            if (!l->getEnable())
                return false;

            return true;
        }
    }
    return false;
}

bool CmdLine::get(std::string name, std::list<int> &list)
{
    for (auto &l : mParamList)
    {
        if (l->getName() == name)
        {
            if (!l->getEnable())
                return false;

            switch (l->getCmdType())
            {
                case CmdType::IntRepeat:
                auto p = std::dynamic_pointer_cast<ParamIntRepeat>(l);
                list = p->getList();
                return true;
                break;
            }

            return true;
        }
    }
    return false;
}

bool CmdLine::get(std::string name, std::string &str)
{
    for (auto &l : mParamList)
    {
        if (l->getName() == name)
        {
            if (!l->getEnable())
                return false;

            switch (l->getCmdType())
            {
                case CmdType::IntRepeat:
                auto p = std::dynamic_pointer_cast<ParamStringNoRange>(l);
                str = p->getStr();
                return true;
                break;
            }

            return true;
        }
    }
    return false;
}

bool CmdLine::parse(int argc, char *argv[])
{
    bool bSearch = false;
    CmdType cmd = CmdType::None;
    std::shared_ptr<ParamBase> pParamBase;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            bSearch = false;
            for (auto &l : mParamList)
            {
                if (l.get()->getName() == argv[i] || 
                    l.get()->getShortName() == argv[i])
                {
                    bSearch = true;
                    l->setEnable();
                    cmd = l->getCmdType();
                    pParamBase = l;
                    if (l->getCmdType() == CmdType::Help)
                    {
                        showHelp();
                        exit(0);
                    }
                }
            }

            if (!bSearch)
            {
                lmc::Logger(LogLevel::clear) << "unrecognized option \"" << argv[i] << "\"";
                showHelp();
                exit(0);
            } 
        }
        else
        {
            switch (cmd)
            {
                case CmdType::IntRepeat:{
                auto p = std::dynamic_pointer_cast<ParamIntRepeat>(pParamBase);
                int num = ::atoi(argv[i]);
                if (p->getMax() < num || p->getMin() > num)
                {
                    lmc::Logger(LogLevel::clear) << "failed: " << argv[i] << " is surpass the limit\"";
                    exit(0);
                }
                p->pushBack(num);
                break; }
                case CmdType::StringNoRange:
                auto p = std::dynamic_pointer_cast<ParamStringNoRange>(pParamBase);
                p->setStr(argv[i]);
                break;
            }
        }
    }

    return true;
}
using namespace std;
void CmdLine::showHelp()
{
    lmc::Logger(LogLevel::clear) << "Usage:";
    lmc::Logger(LogLevel::clear) << "Options:";

    for (auto &l : mParamList)
    {
        std::string tmpStr = "";
        if (l->getCmdType() == CmdType::IntRepeat)
        {
            auto p = std::dynamic_pointer_cast<ParamIntRepeat>(l);
            tmpStr += "[min: " + std::to_string(p->getMin()) + ", max: " + 
                      std::to_string(p->getMax()) + "]";
        }
        std::cout << "    " << l->getShortName() << ", " << std::left << std::setfill(' ') 
                  << std::setw(40) << l->getName() << l->getDescription() << tmpStr << std::endl;
    }
}