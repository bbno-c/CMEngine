#pragma once

class VirtualFileSystem {
public:
    static VirtualFileSystem& GetInstance(const std::string& rootDirectory) {
        static VirtualFileSystem instance(rootDirectory);
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

private:
    VirtualFileSystem(const std::string& rootDirectory) {
        std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
        rootDirectory_ = parentDir +"/"+ rootDirectory;
    }

    std::string rootDirectory_;
};