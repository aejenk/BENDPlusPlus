#include "CalBender.h"

void CalBender::loadFile(std::string filename){
    std::ifstream ifile = std::ifstream(filename, std::ios::in | std::ios::binary | std::ios::ate);

    std::string ss; // ? May be replaced with stringstream

    if(ifile.is_open()){
        fname = filename;
        size_t size = ifile.tellg();
        ifile.seekg(0, std::ios::beg);

        bufferSize = 1024;

        if(size >= 1024*1024*1024){
            // If the file is over a gigabyte, increase buffersize to a megabyte.
            bufferSize *= 1024;
        }

        std::unique_ptr<char[]> buffer (new char[bufferSize]);

        LoadingBar *lbar = new LoadingBar((size/bufferSize));
        lbar->setLabel("Load("+fname+")");

        while(ifile){
            // Reads a chunk of bytes from the file.
            ifile.read(buffer.get(), bufferSize);
            std::string x = std::string(buffer.get(), bufferSize);
            ss.append(x); // ? May be replaced with stringstream
            lbar->nextStep();
        }

        std::cout << "\n";

        // Sets dumb safety minimum to a 50th of a file.
        // This assumes a heuristic that the file is large enough that -
        //  - the first 50th of the bytes contain the entire header.
        // This heuristic may overcompensate a lot, but it avoids the header consistently.
        safetyMin = size / 50;
        contents = ss;

        
        if(contents == ""){
            std::cout << "File is empty. Aborting bender..." << std::endl;
            exit(EXIT_FAILURE);
        }

        _mkdir("output");
        extension = fname.substr(fname.rfind('.'));

        // Used to avoid problems with paths including folders.
        // Without this, if the filename is <folder>/<name>,
        // the output name is output/<folder>/<name>.
        size_t indexFolder = fname.rfind('/');
        indexFolder = (indexFolder == std::string::npos) ? 0 : indexFolder+1;
        fname = fname.substr(indexFolder, fname.rfind('.'));
        
        backup.assign(contents); // backs up file for later restoration
    }
    else{
        std::cout << "No file loaded - are you sure that file exists?" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void CalBender::addMutation(std::string mutname, Mutation *m){
    CalMutation *cm = dynamic_cast<CalMutation*>(m);
    
    // The mutation passed needs to be of type CalMutation or a subclass of it.
    if(cm == nullptr) {
        std::cout << "Couldn't add mutation \"" << mutname << "\" - Mutation passed not of type CalMutation." << std::endl;
        return;
    }

    cm->safetymin = safetyMin;
    mutations.emplace(mutname, cm);
    std::cout << "Added mutation [" << mutname << "]" << std::endl;
}

void CalBender::loadDefaultMutations(){
    std::vector<std::pair<std::string, Mutation*>> defaults = {
        {"RANDOM", new ChunkRandomize()},
        {"MOVE", new MoveChunks()},
        {"REPEAT", new RepeatChunks()},
        {"REMOVE", new RemoveChunks()},
        {"REVERSE", new ReverseChunks()},
        {"NULL", new NullChunks()},
        {"SWAP", new SwapChunks()},
        {"INCREMENT", new IncrementBytes()},
        {"RAINBOW", new RainbowSpread()},
        {"ECHO", new EchoWave()},
        {"AVERAGE", new AverageChunks()}
    };

    for(auto mut: defaults){
        this->addMutation(mut.first, mut.second);
    }
}

void CalBender::mutateUsing(std::string mutname, std::map<std::string, std::any> options){
    this->muthistory += this->mutations[mutname]->to_string(options);
    this->mutations[mutname]->mutate(contents, options);
    std::cout << "\n";
}

void CalBender::setGlobalMutationOptions(std::map<std::string, std::string> options){
    mutoptions = options;
}

std::map<std::string, std::string> CalBender::getGlobalMutationOptions(){
    return mutoptions;
}

void CalBender::mutateUsing(std::string mutname){
    this->mutateUsing(mutname, this->mutoptions);
}

void CalBender::mutateUsing(std::string mutname, std::map<std::string, std::string> options){
    std::map<std::string, std::any> generatedOptions;

    for(auto option: options){
        // Gets the range of the option.
        std::string optrange = option.second;

        // Gets the minimum and maximum of the option range in string form.
        std::vector<std::string> minmax = Utility::split(optrange, '-');

        // Checks whether the number is a double or not by finding a period.
        // This operates on the assumption that in the options,
        //      the only valid option with a period would be a doubletyped option.
        bool isDouble = optrange.find('.') != std::string::npos;

        // If no range was detected, add the option directly.
        if(minmax.size() == 1){
            (isDouble) ? generatedOptions.emplace(option.first, stod(minmax[0]))
                       : generatedOptions.emplace(option.first, stoi(minmax[0]));
        }
        // If the numbers were found to be double, add a generated double number.
        else if(isDouble){
            realRand = std::uniform_real_distribution(stod(minmax[0]), stod(minmax[1]));
            generatedOptions.emplace(option.first, realRand(optionGenerator));
        }
        // If they were found to be integers, add a generated integer number.
        else {
            intRand = std::uniform_int_distribution(stoi(minmax[0]), stoi(minmax[1]));
            generatedOptions.emplace(option.first, intRand(optionGenerator));   
        }
    }

    this->mutateUsing(mutname, generatedOptions);
}

void CalBender::saveContents(){
    std::stringstream ss;
    ss << "output/" << fname << muthistory << extension;
    std::string savefile = ss.str();

    // Opens a file with [filename]
    std::ofstream ofile (savefile, std::ios::out | std::ios::binary);

    size_t filesize = contents.size();
    LoadingBar *lbar = new LoadingBar(filesize/bufferSize);

    // Buffers contents into file according to amount of buffers used to load file.
    for(int i = 0; (i*bufferSize) < filesize; i++){
        // Saves a chunk of bytes into the file.
        std::string bufferstr = contents.substr(i*bufferSize, bufferSize);
        ofile << bufferstr;
        lbar->nextStep();
    }

    std::cout << "\n";

    std::cout << "\tSaved [" << savefile << "]" << std::endl;
}

void CalBender::resetFile(){
    std::cout << "Resetting file to last backup..." << std::endl;
    contents.assign(backup);
    muthistory.clear();
}