#include "CalMutations.h"

void CalMutation::mutate(std::string &contents, std::map<std::string, std::any> options) {};
std::string CalMutation::to_string(std::map<std::string, std::any> options) {return "<error> this shouldn't appear"; };

void ChunkRandomize::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<int>(options["iterations"]);
    long chunksize = std::any_cast<int>(options["chunksize"]);

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Chunks");

    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = indexDist(generator);

        for(int j = rindex; j < chunksize + rindex; j++){
            contents[j] = static_cast<char>(rand() % 256);
        }

        lb->nextStep();
    }
}

std::string ChunkRandomize::to_string(std::map<std::string, std::any> options) {
    std::stringstream ss;
    ss  << "_random"
        << "_it="
        << std::any_cast<int>(options["iterations"])
        << "_chk="
        << std::any_cast<int>(options["chunksize"]);
    
    return ss.str();
}

void MoveChunks::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<int>(options["iterations"]);
    long chunksize = std::any_cast<int>(options["chunksize"]);

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Move");

    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = indexDist(generator);

        std::string sub = contents.substr(rindex, chunksize);
        contents.erase(rindex, chunksize);

        rindex = indexDist(generator);
        contents.insert(rindex, sub);

        lb->nextStep();
    }
}

std::string MoveChunks::to_string(std::map<std::string, std::any> options) {
    std::stringstream ss;
    ss  << "_move"
        << "_it="
        << std::any_cast<int>(options["iterations"])
        << "_chk="
        << std::any_cast<int>(options["chunksize"]);
    
    return ss.str();
}

void RepeatChunks::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<int>(options["iterations"]);
    long chunksize = std::any_cast<int>(options["chunksize"]);
    long repeats = std::any_cast<int>(options["repeats"]);

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Repeat");

    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = indexDist(generator);
        std::string randomchunk = contents.substr(rindex, chunksize);
        
        for(int r = 0; r < repeats && rindex+chunksize < contents.length(); r++){
            rindex += chunksize;
            contents = contents.replace(rindex, chunksize, randomchunk);
        }

        lb->nextStep();
    }
}

std::string RepeatChunks::to_string(std::map<std::string, std::any> options) {
    std::stringstream ss;
    ss  << "_repeat"
        << "_it="
        << std::any_cast<int>(options["iterations"])
        << "_chk="
        << std::any_cast<int>(options["chunksize"])
        << "_rep="
        << std::any_cast<int>(options["repeats"]);
    
    return ss.str();
}

void RemoveChunks::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<int>(options["iterations"]);
    long chunksize = std::any_cast<int>(options["chunksize"]);

    if(iter*chunksize >= contents.length()){
        std::cout << "Remove mutation failed. Amount to be deleted over filesize." << std::endl;
        return;
    }

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Remove");

    size_t rindex;

    size_t min = indexDist.min();
    size_t max = indexDist.max();

    for(int i = 0; i < iter; i++){
        rindex = indexDist(generator);
        contents.erase(rindex, chunksize);

        max -= chunksize;
        indexDist = std::uniform_int_distribution<size_t>(min, max);
        lb->nextStep();
    }
}

std::string RemoveChunks::to_string(std::map<std::string, std::any> options) {
    std::stringstream ss;
    ss  << "_remove"
        << "_it="
        << std::any_cast<int>(options["iterations"])
        << "_chk="
        << std::any_cast<int>(options["chunksize"]);
    
    return ss.str();
}

void ReverseChunks::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<int>(options["iterations"]);
    long chunksize = std::any_cast<int>(options["chunksize"]);

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Reverse");

    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = indexDist(generator);

        std::string chunk = contents.substr(rindex, chunksize);
        std::reverse(chunk.begin(), chunk.end());
        contents = contents.replace(rindex, chunksize, chunk);

        lb->nextStep();
    }
}

std::string ReverseChunks::to_string(std::map<std::string, std::any> options) {
    std::stringstream ss;
    ss  << "_reverse"
        << "_it="
        << std::any_cast<int>(options["iterations"])
        << "_chk="
        << std::any_cast<int>(options["chunksize"]);
    
    return ss.str();
}

void NullChunks::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<int>(options["iterations"]);
    long chunksize = std::any_cast<int>(options["chunksize"]);

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Null");

    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = indexDist(generator);

        for(int j = rindex; j < chunksize + rindex; j++){
            contents[j] = 0x00;
        }

        lb->nextStep();
    }
}

std::string NullChunks::to_string(std::map<std::string, std::any> options) {
    std::stringstream ss;
    ss  << "_null"
        << "_it="
        << std::any_cast<int>(options["iterations"])
        << "_chk="
        << std::any_cast<int>(options["chunksize"]);
    
    return ss.str();
}

void SwapChunks::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<int>(options["iterations"]);
    long chunksize = std::any_cast<int>(options["chunksize"]);

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Swap");

    size_t rindex1;
    size_t rindex2;

    for(int i = 0; i < iter; i++){
        rindex1 = indexDist(generator);
        rindex2 = indexDist(generator);
        std::string sub1 = contents.substr(rindex1, chunksize);
        std::string sub2 = contents.substr(rindex2, chunksize); 
    
        contents.replace(rindex1, chunksize, sub2);
        contents.replace(rindex2, chunksize, sub1);

        lb->nextStep();
    }
}

std::string SwapChunks::to_string(std::map<std::string, std::any> options) {
    std::stringstream ss;
    ss  << "_swap"
        << "_it="
        << std::any_cast<int>(options["iterations"])
        << "_chk="
        << std::any_cast<int>(options["chunksize"]);
    
    return ss.str();
}

void IncrementBytes::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<int>(options["iterations"]);
    long chunksize = std::any_cast<int>(options["chunksize"]);
    long incby = std::any_cast<int>(options["incrementby"]);

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Increment");

    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = indexDist(generator);

        for(int j = rindex; j < chunksize+rindex; j++){
            contents[j] += incby;
        }

        lb->nextStep();
    }
}

std::string IncrementBytes::to_string(std::map<std::string, std::any> options) {
    std::stringstream ss;
    ss  << "_increment"
        << "_it="
        << std::any_cast<int>(options["iterations"])
        << "_chk="
        << std::any_cast<int>(options["chunksize"])
        << "_incby="
        << std::any_cast<int>(options["incrementby"]);
    
    return ss.str();
}

void RainbowSpread::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<int>(options["iterations"]);
    long chunksize = std::any_cast<int>(options["chunksize"]);
    long raindelay = std::any_cast<int>(options["raindelay"]);
    long rainsize = std::any_cast<int>(options["rainsize"]);

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Rainbow");

    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = indexDist(generator);

        for(int j = rindex; j < chunksize+rindex; j++){
            contents[j] += floor((j-rindex)/raindelay) * rainsize;
        }

        lb->nextStep();
    }
}

std::string RainbowSpread::to_string(std::map<std::string, std::any> options) {
    std::stringstream ss;
    ss  << "_rainbow"
        << "_it="
        << std::any_cast<int>(options["iterations"])
        << "_chk="
        << std::any_cast<int>(options["chunksize"])
        << "_rdelay="
        << std::any_cast<int>(options["raindelay"])
        << "_rsize="
        << std::any_cast<int>(options["rainsize"]);
    
    return ss.str();
}

void EchoWave::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<int>(options["iterations"]);
    long chunksize = std::any_cast<int>(options["chunksize"]);
    double echodecay = std::any_cast<double>(options["echodecay"]);
    long echolength = std::any_cast<int>(options["echolength"]);

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Echo");

    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = indexDist(generator);

        for(int j = rindex; j < chunksize+rindex; j++){
            float k_decay = echodecay;
            for(int k = j-1; k >= j-echolength && k >= 0; k--){
                contents[j] += contents[k]*k_decay;
                k_decay *= echodecay;
            }
        }

        lb->nextStep();
    }
}

std::string EchoWave::to_string(std::map<std::string, std::any> options) {
    std::stringstream ss;
    ss  << "_echo"
        << "_it="
        << std::any_cast<int>(options["iterations"])
        << "_chk="
        << std::any_cast<int>(options["chunksize"])
        << "_decay="
        << std::any_cast<double>(options["echodecay"])
        << "_len="
        << std::any_cast<int>(options["echolength"]);
    
    return ss.str();
}

void AverageChunks::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<int>(options["iterations"]);
    long chunksize = std::any_cast<int>(options["chunksize"]);

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Average");

    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = indexDist(generator);

        long avg = 0;
        for(int j = rindex; j < chunksize+rindex; j++){
            avg += contents[j];
        }
        avg /= chunksize;
        for(int j = rindex; j < chunksize+rindex; j++){
            contents[j] = avg;
        }

        lb->nextStep();
    }
}

std::string AverageChunks::to_string(std::map<std::string, std::any> options) {
    std::stringstream ss;
    ss  << "_average"
        << "_it="
        << std::any_cast<int>(options["iterations"])
        << "_chk="
        << std::any_cast<int>(options["chunksize"]);
    
    return ss.str();
}

void Inversion::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<int>(options["iterations"]);
    long chunksize = std::any_cast<int>(options["chunksize"]);

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Inversion");

    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = indexDist(generator);

        long avg = 0;
        for(int j = rindex; j < chunksize+rindex; j++){
            contents[j] = 255 - contents[j];
        }

        lb->nextStep();
    }
}

std::string Inversion::to_string(std::map<std::string, std::any> options) {
    std::stringstream ss;
    ss  << "_invert"
        << "_it="
        << std::any_cast<int>(options["iterations"])
        << "_chk="
        << std::any_cast<int>(options["chunksize"]);
    
    return ss.str();
}

void DragScratch::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<int>(options["iterations"]);
    long chunksize = std::any_cast<int>(options["chunksize"]);

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Drag");

    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = indexDist(generator);

        char dragger = contents[rindex];

        for(int j = rindex+1; j < chunksize+rindex; j++){
            contents[j] = dragger;
        }

        lb->nextStep();
    }
}

std::string DragScratch::to_string(std::map<std::string, std::any> options) {
    std::stringstream ss;
    ss  << "_drag"
        << "_it="
        << std::any_cast<int>(options["iterations"])
        << "_chk="
        << std::any_cast<int>(options["chunksize"]);
    
    return ss.str();
}

void PixelSort::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<int>(options["iterations"]);
    long chunksize = std::any_cast<int>(options["chunksize"]);

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Sort");

    size_t rindex;

    for(int i = 0; i < iter; i++){
        do{
            rindex = indexDist(generator);
        } while(chunksize+rindex > contents.size());
        char dragger = contents[rindex];
        std::sort(contents.begin()+rindex, contents.begin()+rindex+chunksize);
        lb->nextStep();
    }
}

std::string PixelSort::to_string(std::map<std::string, std::any> options) {
    std::stringstream ss;
    ss  << "_sort"
        << "_it="
        << std::any_cast<int>(options["iterations"])
        << "_chk="
        << std::any_cast<int>(options["chunksize"]);
    
    return ss.str();
}

void Magnifi::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<int>(options["iterations"]);
    long chunksize = std::any_cast<int>(options["chunksize"]);
    double factor = std::any_cast<double>(options["magnify"]);

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Magnifi");

    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = indexDist(generator);

        for(int j = rindex+1; j < chunksize+rindex; j++){
            contents[j] *= factor;
        }

        lb->nextStep();
    }
}

std::string Magnifi::to_string(std::map<std::string, std::any> options) {
    std::stringstream ss;
    ss  << "_magnifi"
        << "_it="
        << std::any_cast<int>(options["iterations"])
        << "_chk="
        << std::any_cast<int>(options["chunksize"])
        << "_fac="
        << std::any_cast<double>(options["magnify"]);
    
    return ss.str();
}