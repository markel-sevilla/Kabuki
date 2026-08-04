#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "Log.h"
namespace Debug {

// interfaz base 
class ISink {
public:
    virtual ~ISink() = default;
    virtual void write(const std::string& message, DebuggingInstance::LogLevel level) = 0;
};

// ConsoleSink: it writes logs to the console 
class ConsoleSink : public ISink {
public:
   
    void write(const std::string& message, DebuggingInstance::LogLevel level) override {
        
        if (level == DebuggingInstance::LogLevel::Error || level == DebuggingInstance::LogLevel::Fatal)
            throw std::runtime_error{ message };
    
        std::cout << message << std::endl;
    }
};

// FileSink: it writes logs to a file
class FileSink : public ISink {
public:

    //don't want to use the constructor without a file path, so we make it explicit
    explicit FileSink(const std::string& filepath) 
    {
        mFile.open(filepath, std::ios::app);
        if (!mFile.is_open())
            std::cerr << "FileSink: could not open " << filepath << "\n";
    }
    
    // Virtual destructor fot the File sink
    ~FileSink() override {
        if (mFile.is_open())
            mFile.close();
    }

    void write(const std::string& message, DebuggingInstance::LogLevel level) override {
        if (mFile.is_open())
            mFile << DebuggingInstance::get()->levelToString(level) << " " << message << "\n";
    }

private:
    std::ofstream mFile;
};

} // namespace Debug