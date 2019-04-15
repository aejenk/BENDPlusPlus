#include "mutations.h"
#include "timetaker.cpp"

std::default_random_engine generator;

using namespace std;

/* Public functions */

void Mutation::initDists() {
    csize = uniform_int_distribution<size_t>(rchunksize.first, rchunksize.second);
    reps = uniform_int_distribution<size_t>(rrepeats.first, rrepeats.second);
    itr = uniform_int_distribution<size_t>(riters.first, riters.second);
    incbyd = uniform_int_distribution<size_t>(rincby.first, rincby.second);
    raindelayd = uniform_int_distribution<size_t>(rraindelay.first, rraindelay.second);
    rainsized = uniform_int_distribution<size_t>(rrainsize.first, rrainsize.second);
    decayd = uniform_real_distribution<float>(rdecay.first, rdecay.second);
    persistd = uniform_int_distribution<size_t>(rpersist.first, rpersist.second);
}

template<typename T>
T Mutation::getOptionGenerator(OPTIONS opt){
    switch(opt){
        case OPTIONS::CHUNKSIZE :
            return (rchunksize.first == rchunksize.second) ? rchunksize.first : csize(generator);
        case OPTIONS::REPEATS :
            return (rrepeats.first == rrepeats.second) ? rrepeats.first : reps(generator);
        case OPTIONS::ITERS :
            return (riters.first == riters.second) ? riters.first : itr(generator);
        case OPTIONS::INC_BY :
            return (rincby.first == rincby.second) ? rincby.first : incbyd(generator);
        case OPTIONS::RAIN_DELAY :
            return (rraindelay.first == rraindelay.second) ? rraindelay.first : raindelayd(generator);
        case OPTIONS::RAIN_SIZE :
            return (rrainsize.first == rrainsize.second) ? rrainsize.first : rainsized(generator);
        case OPTIONS::DECAY :
            return (rdecay.first == rdecay.second) ? rdecay.first : decayd(generator);
        case OPTIONS::PERSIST :
            return (rpersist.first == rpersist.second) ? rpersist.first : persistd(generator);
        default :
            return 0;
    }
}

void Mutation::mutate(muts mutation, size_t safetybuf, string& contents){
    size_t len = contents.length();
    size_t rindex;

    size_t min = safetybuf;
    size_t max;

    chunksize = getOptionGenerator(OPTIONS::CHUNKSIZE);
    iter = getOptionGenerator(OPTIONS::ITERS);

    if(mutation == muts::SCATTER){
        repeats = getOptionGenerator(OPTIONS::REPEATS);
    }
    else if(mutation == muts::INCREMENT){
        incby = getOptionGenerator(OPTIONS::INC_BY);
    }
    else if(mutation == muts::RAINBOW){
        raindelay = getOptionGenerator(OPTIONS::RAIN_DELAY);
        rainsize = getOptionGenerator(OPTIONS::RAIN_SIZE);
    }
    else if(mutation == muts::ECHO){
        decay = getOptionGenerator<float>(OPTIONS::DECAY);
        persist = getOptionGenerator(OPTIONS::PERSIST);
    }

    int r_id = rand()%1000; // to avoid overwriting
    mutstr += "-RID=" + to_string(r_id);

    switch(mutation){
        // normal modes
        case muts::SCATTER      :
        case muts::ZERO         : max = len; break;
        // chunk modes
        case muts::CHUNKS       :
        case muts::MOVE         :
        case muts::REMOVE       :
        case muts::SWAP         :
        case muts::ISWAP        :
        case muts::INCREMENT    :
        case muts::RAINBOW      :
        case muts::ECHO         :
        case muts::AVERAGE      :
        case muts::REVERSE      : max = len-chunksize; break;
        // other modes
        case muts::REPEAT       : max = len - (chunksize*repeats); break;
    }

    dist = uniform_int_distribution<size_t>(min,max);

    loadingBar = LoadingBar(iter);

       // switch with time calculations.
    switch(mutation){
        case muts::SCATTER      :     takeTimeWithoutReturn("SCT", mutscatter(contents)); break;
        case muts::CHUNKS       :     takeTimeWithoutReturn("CHKS", mutchunks(contents)); break;
        case muts::MOVE         :        takeTimeWithoutReturn("MOV", mutmove(contents)); break;
        case muts::REMOVE       :      takeTimeWithoutReturn("REM", mutremove(contents)); break;
        case muts::REVERSE      :     takeTimeWithoutReturn("REV", mutreverse(contents)); break;
        case muts::REPEAT       :      takeTimeWithoutReturn("REP", mutrepeat(contents)); break;
        case muts::ZERO         :        takeTimeWithoutReturn("ZER", mutzero(contents)); break;
        case muts::SWAP         :        takeTimeWithoutReturn("SWP", mutswap(contents)); break;
        case muts::ISWAP        :       takeTimeWithoutReturn("IWP", mutiswap(contents)); break;
        case muts::INCREMENT    :   takeTimeWithoutReturn("INC", mutincrement(contents)); break;
        case muts::RAINBOW      :     takeTimeWithoutReturn("RBW", mutrainbow(contents)); break;
        case muts::ECHO         :        takeTimeWithoutReturn("ECH", mutecho(contents)); break;
        case muts::AVERAGE      :     takeTimeWithoutReturn("AVG", mutaverage(contents)); break;
    }

    cout << endl;
}


/* Private functions */
void Mutation::mutscatter(string& contents){
    loadingBar.setLabel("Scatter");
    size_t rindex;

    mutstr += "-SCT-i=" + to_string(iter);
        
    for(int i = 0; i < iter; i++){
        rindex = dist(generator);
        contents[rindex] = randomASCII();
        loadingBar.nextStep();
    }
}

void Mutation::mutchunks(string& contents){
    mutstr += "-CHK-i=" + to_string(iter) + "-c=" + to_string(chunksize);
    loadingBar.setLabel("Chunks");
    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = dist(generator);

        for(int j = rindex; j < chunksize+rindex; j++){
            contents[j] = randomASCII();
        }

        loadingBar.nextStep();
    }
}

void Mutation::mutincrement(string& contents){
    mutstr += "-INC-i=" + to_string(iter) + "-c=" + to_string(chunksize) + "-inc=" + to_string(incby);
    loadingBar.setLabel("Increment");
    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = dist(generator);

        for(int j = rindex; j < chunksize+rindex; j++){
            contents[j] += incby;
        }

        loadingBar.nextStep();
    }
}

void Mutation::mutrainbow(string& contents){
    mutstr += "-RBW-i=" + to_string(iter) + "-c=" + to_string(chunksize) + "-rd=" + to_string(raindelay) + "-rs=" + to_string(rainsize);
    loadingBar.setLabel("Rainbow");
    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = dist(generator);

        for(int j = rindex; j < chunksize+rindex; j++){
            contents[j] += floor((j-rindex)/raindelay) * rainsize;
        }

        loadingBar.nextStep();
    }
}

void Mutation::mutaverage(string& contents){
    mutstr += "-AVG-i=" + to_string(iter) + "-c=" + to_string(chunksize);
    loadingBar.setLabel("Average");
    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = dist(generator);

        long avg = 0;
        for(int j = rindex; j < chunksize+rindex; j++){
            avg += contents[j];
        }
        avg /= chunksize;
        for(int j = rindex; j < chunksize+rindex; j++){
            contents[j] = avg;
        }

        loadingBar.nextStep();
    }
}

void Mutation::mutecho(string& contents){
    mutstr += "-ECH-i=" + to_string(iter) + "-c=" + to_string(chunksize) + "-ed=" + to_string(decay) + "-rs=" + to_string(persist);
    loadingBar.setLabel("Echo");
    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = dist(generator);

        for(int j = rindex; j < chunksize+rindex; j++){
            float k_decay = decay;
            for(int k = j-1; k >= j-persist && k >= 0; k--){
                contents[j] += contents[k]*k_decay;
                k_decay *= decay;
            }
        }

        loadingBar.nextStep();
    }
}

void Mutation::mutrepeat(string& contents){
    mutstr += "-REP-i=" + to_string(iter) + "-c=" + to_string(chunksize) + "-r=" + to_string(repeats);
    loadingBar.setLabel("Repeat");
    int len = contents.length();
    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = dist(generator);
        string randomchunk = contents.substr(rindex, chunksize);
        
        for(int r = 0; r < repeats && rindex+chunksize < len; r++){
            rindex += chunksize;
            contents = contents.replace(rindex, chunksize, randomchunk);
        }

        loadingBar.nextStep();
    }
}

void Mutation::mutreverse(string& contents){
    mutstr += "-REV-i=" + to_string(iter) + "-c=" + to_string(chunksize);
    loadingBar.setLabel("Reverse");
    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = dist(generator);

        string chunk = contents.substr(rindex, chunksize);
        reverse(chunk.begin(), chunk.end());
        contents = contents.replace(rindex, chunksize, chunk);

        loadingBar.nextStep();
    }
}

void Mutation::mutremove(string& contents){
    if(chunksize*iter >= contents.length()){
        mutstr += "-ERROR-REM";
        return;
    }
    mutstr += "-REM-i=" + to_string(iter) + "-c=" + to_string(chunksize);
    loadingBar.setLabel("Remove");
    size_t rindex;

    size_t min = dist.min();
    size_t max = dist.max();

    for(int i = 0; i < iter; i++){
        rindex = dist(generator);
        contents.erase(rindex, chunksize);

        max -= chunksize;
        dist = uniform_int_distribution<size_t>(min, max);
        loadingBar.nextStep();
    }
}

void Mutation::mutmove(string& contents){
    mutstr += "-MOV-i=" + to_string(iter) + "-c=" + to_string(chunksize);
    loadingBar.setLabel("Move");
    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = dist(generator);

        string sub = contents.substr(rindex, chunksize);
        contents.erase(rindex, chunksize);

        rindex = dist(generator);
        contents.insert(rindex, sub);

        loadingBar.nextStep();
    }
}

void Mutation::mutiswap(string& contents){
    mutstr += "-ISW-i=" + to_string(iter) + "-c=" + to_string(chunksize);
    loadingBar.setLabel("Iterswap");
    size_t rbegin;
    size_t rbegin2;
    size_t rend;

    for(int i = 0; i < iter; i++){
        rbegin = dist(generator);
        rbegin2 = dist(generator);
        rend = rbegin + chunksize;

        swap_ranges(contents.begin()+rbegin, contents.begin()+rend, contents.begin()+rbegin2);
        loadingBar.nextStep();
    }
}

void Mutation::mutzero(string& contents){
    mutstr += "-ZER-i=" + to_string(iter);
    loadingBar.setLabel("Zero");
    size_t rindex;
        
    for(int i = 0; i < iter; i++){
        rindex = dist(generator);
        contents[rindex] = 0x00;

        loadingBar.nextStep();
    }
}

void Mutation::mutswap(string& contents){
    mutstr += "-SWP-i=" + to_string(iter) + "-c=" + to_string(chunksize);
    loadingBar.setLabel("Swap");
    size_t rindex1;
    size_t rindex2;
    
    cout << "Mutating [SWAP]";

    for(int i = 0; i < iter; i++){
        rindex1 = dist(generator);
        rindex2 = dist(generator);
        string sub1 = contents.substr(rindex1, chunksize);
        string sub2 = contents.substr(rindex2, chunksize); 

        contents.replace(rindex1, chunksize, sub2);
        contents.replace(rindex2, chunksize, sub1);

        loadingBar.nextStep();
    }
}

char Mutation::randomASCII(){
    return static_cast<char>(rand() % 256);
}

string Mutation::getMutString() {
    return mutstr;
}

void Mutation::resetMut() {
    mutstr = "";
}