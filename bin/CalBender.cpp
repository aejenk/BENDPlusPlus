#include "CalBender.h"


void CalBender::loadFile(std::string filename){
    std::ifstream ifile = std::ifstream(filename, ios::in | ios::binary | ios::ate);

    string ss; // ? May be replaced with stringstream

    if(ifile.is_open()){
        fname = filename;
        size_t size = ifile.tellg();
        ifile.seekg(0, ios::beg);

        bufferSize = 1024;

        if(size >= 1024*1024*1024){
            // If the file is over a gigabyte, increase buffersize to a megabyte.
            bufferSize *= 1024;
        }

        unique_ptr<char[]> buffer (new char[bufferSize]);

        LoadingBar lbar = LoadingBar((size/bufferSize));
        lbar.setLabel("Load("+fname+")");

        while(ifile){
            // Reads a chunk of bytes from the file.
            ifile.read(buffer.get(), bufferSize);
            string x = string(buffer.get(), bufferSize);
            ss.append(x); // ? May be replaced with stringstream
            lbar.nextStep();
        }

        CLEARLINE();

        // Sets dumb safety minimum to a 50th of a file.
        // This assumes a heuristic that the file is large enough that -
        //  - the first 50th of the bytes contain the entire header.
        // This heuristic may overcompensate a lot, but it avoids the header consistently.
        safetyMin = size / 50;
        contents = ss;

        
        if(contents == ""){
            cout << "File is empty. Aborting bender..." << endl;
            exit(EXIT_FAILURE);
        }

        _mkdir("output");
        extension = fname.substr(fname.rfind('.'));

        // Used to avoid problems with paths including folders.
        // Without this, if the filename is <folder>/<name>,
        // the output name is output/<folder>/<name>.
        size_t indexFolder = fname.rfind('/');
        indexFolder = (indexFolder == string::npos) ? 0 : indexFolder+1;
        fname = fname.substr(indexFolder, fname.rfind('.'));
        
        backup.assign(contents); // backs up file for later restoration
    }
    else{
        cout << "No file loaded - are you sure that file exists?" << endl;
        exit(EXIT_FAILURE);
    }
}

void CalBender::addMutation(std::string mutname, Mutation *m){
    CalMutation *cm = dynamic_cast<CalMutation*>(m);
    
    // The mutation passed needs to be of type CalMutation or a subclass of it.
    if(cm = nullptr) {
        cout << "Couldn't add mutation \"" << mutname << "\" - Mutation passed not of type CalMutation." << endl;
        return;
    }

    cm->safetymin = safetyMin;
    mutations.emplace(mutname, cm);
}

void CalBender::mutateUsing(std::string mutname, std::map<std::string, std::any> options){
    this->mutations[mutname]->mutate(contents, options);
}

void CalBender::saveContents(){
    stringstream ss;
    ss << "output/" << fname << extension;
    string savefile = ss.str();

    // Opens a file with [filename]
    ofstream ofile (savefile, ios::out | ios::binary);

    size_t filesize = contents.size();
    LoadingBar lbar = LoadingBar(filesize/bufferSize);

    // Buffers contents into file according to amount of buffers used to load file.
    for(int i = 0; (i*bufferSize) < filesize; i++){
        // Saves a chunk of bytes into the file.
        string bufferstr = contents.substr(i*bufferSize, bufferSize);
        ofile << bufferstr;
        lbar.nextStep();
    }

    CLEARLINE();

    cout << "\tSaved [" << savefile << "]" << endl;
}