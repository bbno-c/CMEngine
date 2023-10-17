#pragma once

class VirtualFileSystem {
public:
    static VirtualFileSystem& GetInstance() {
        static VirtualFileSystem instance;
        return instance;
    }

    // Function to load a file from the VFS
    bool LoadFile(const std::string& filename, std::string& content) {
        // Construct the full path to the file
        std::string fullPath = rootDirectory_ + filename;

        std::ifstream file(fullPath);
        if (!file.is_open()) {
            std::cerr << "Error: File not found - " << fullPath << std::endl;
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        content = buffer.str();

        file.close();
        return true;
    }

    std::string GetVFSFilePath(const std::string& filePath) {
        return rootDirectory_ + filePath;
    }

    void Init(const std::string& rootDirectory) {
        std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
        rootDirectory_ = parentDir + "/" + rootDirectory;
    }

private:
    VirtualFileSystem() : rootDirectory_("") {
    }

    std::string rootDirectory_;
};