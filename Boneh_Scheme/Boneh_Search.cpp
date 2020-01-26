//
// Created by Quinn Sheng on 2020/1/18.
//
#include "Boneh_Search.h"

void HomoMulit(
        pcs_public_key *pk,
        hcs_random *securityParam,
        int times,
        ZZZ rop) {
    for (int i = 0; i < times; i++) {
        pcs_encrypt(pk, securityParam,
                    rop.get_mpz_t(),
                    rop.get_mpz_t());
    }
}

ZZZ Add_Homo_1(ZZZ a, ZZZ b, pcs_public_key *pk) {
    ZZZ e, N2;
    string tmpStr = mpz_get_str(NULL, 10, pk->n2);
    N2 = tmpStr;

    e = (a * b) % N2;

    return e;

}

ZZZ Add_Homo_2(ZZZ e, ZZZ l, pcs_public_key *pk) {

    mpz_powm(e.get_mpz_t(), e.get_mpz_t(), l.get_mpz_t(), pk->n2);

    return e;
}

void Search(const char *outName,
            const char *epmName,
            pcs_public_key *pk,
            vector<string> KeysHashTable,
            vector<string> searchToken,
            int PolyNum, int MaxIdNum,
            hcs_random *securityParam) {

    // read encrypted polynomial matrix
    std::ifstream ifs(epmName);
    if (!ifs) {
        std::cout << "<Search> open '" << epmName << "' error." << std::endl;
        abort();
    }
    int col = MaxIdNum + 1;
    ZZZ **ematrix = new ZZZ *[PolyNum];
    ZZZ *edata = new ZZZ[PolyNum * col];
    for (int i = 0; i < PolyNum; i++) {
        ematrix[i] = &edata[i * col];
    }

    std::string strline;
    for (int i = 0; i < PolyNum; i++) {
//        cout << "# " << i << endl;
        std::getline(ifs, strline);
        std::vector<std::string> items;
        split(items, strline, " ");
        for (int j = 0; j < col; j++) {
            ematrix[i][j] = items[j];
        }
    }
    ifs.close();

    // read encrypted search token, get Index | searchToken
    cout << "ST num: " << searchToken.size() << " KH num: " << KeysHashTable.size() << endl;
    vector<int> kwIndex;
    for (int i = 0; i < searchToken.size(); ++i) {
        for (int j = 0; j < KeysHashTable.size(); ++j) {
            if (searchToken[i] == KeysHashTable[j]) {
                cout << "Got match: #" << j << " Value: " << KeysHashTable[j] << endl;
                kwIndex.push_back(j);
            }
        }
    }

    cout << "Start-Search-part" << endl;
    // calculate encrypted polynomial matrix times encrypted search token

    // generate random polynomial
//       initialize random seed:
    srand(time(NULL));

    // create digit plaintext R/R'
    ZZZ **dmatrix = new ZZZ *[PolyNum];
    ZZZ *ddata = new ZZZ[PolyNum * col];
    for (int i = 0; i < PolyNum; i++) {
        dmatrix[i] = &ddata[i * col];
    }
    //  TEST
    for (int i = 0; i < PolyNum; i++) {
        int randNum = rand() % PolyNum + 1;
        for (int j = 0; j < col; j++) {
            dmatrix[i][j] = randNum;
        }
    }

    // calculate new encrypted polynomial matrix
    int newcol = col + col + 1;
    ZZZ **NewPolyMatrix = new ZZZ *[PolyNum];
    ZZZ *NewPolyData = new ZZZ[PolyNum * newcol];
    for (int i = 0; i < PolyNum; i++) {
        NewPolyMatrix[i] = &NewPolyData[i * newcol];
    }

    timeval start, end;
    gettimeofday(&start, 0);

    if (kwIndex.size() != 0) {
        int t = kwIndex.size();
        int c = 1;
        //  2<= i <= t
        for (int i = 0; i < t - 1; ++i) {
            int polyIndex = kwIndex[i];
            c = rand() % PolyNum + 1;
            //  cA_i
            for (int j = 0; j < col; j++) {
                HomoMulit(pk, securityParam, c, ematrix[polyIndex][j]);
            }
        }
        //  Calculate A' & \sum{c_iA_i}
        for (int i = 1; i < t - 1; ++i) {
            int polyIndex = kwIndex[i];
            for (int j = 0; j < col; ++j) {
                ematrix[kwIndex[t - 1]][j] =
                        Add_Homo_1(ematrix[kwIndex[t - 1]][j],
                                   ematrix[polyIndex][j],
                                   pk);
            }
        }
        //  Calculate B
        //  A_1\times R_1
        int dcol = col;
        for (int j = 0; j < newcol; j++) {
            NewPolyMatrix[0][j] = 1;
            for (int k = 0; k <= j; k++) {
                if (k < col && j - k < dcol) {
//                    cout << "EMatrix: " << ematrix[0][k] << endl;
                    NewPolyMatrix[0][j] = Add_Homo_1(
                            NewPolyMatrix[0][j],
                            Add_Homo_2(ematrix[0][k],
                                       dmatrix[0][j - k], pk),
                            pk);
                }
            }
        }
        //  A'\times R'
        int kwLastIndex = kwIndex[t - 1];
        for (int j = 0; j < newcol; j++) {
            NewPolyMatrix[kwLastIndex][j] = 1;
            for (int k = 0; k <= j; k++) {
                if (k < col && j - k < dcol) {
                    NewPolyMatrix[kwLastIndex][j] = Add_Homo_1(
                            NewPolyMatrix[kwLastIndex][j],
                            Add_Homo_2(ematrix[0][k],
                                       dmatrix[0][j - k],
                                       pk),
                            pk);
                }
            }
        }
        //  B: Add; get result
        for (int j = 0; j < newcol; j++) {
            NewPolyMatrix[0][j] = Add_Homo_1(NewPolyMatrix[kwLastIndex][j],
                                             NewPolyMatrix[0][j],
                                             pk);
        }
    }

    gettimeofday(&end, 0);
    double cost_time_msec = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;

    std::ofstream ofs(outName);
    if (!ofs) {
        std::cout << "<Search> open '" << outName << "' error." << std::endl;
        abort();
    }
    for (int i = 0; i < PolyNum; i++) {
        for (int j = 0; j < newcol; j++) {
            ofs << (j ? " " : "") << NewPolyMatrix[i][j];
        }
        ofs << std::endl;
    }
    ofs.close();

    delete[] ematrix;
    delete[] edata;
    delete[] NewPolyMatrix;
    delete[] NewPolyData;

    cout << "Searching Time: ";
    cout << cost_time_msec << "ms" << endl;
}
