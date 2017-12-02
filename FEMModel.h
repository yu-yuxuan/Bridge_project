#ifndef FEMMODEL_H
#define FEMMODEL_H

class FEMModel
{
protected:
    int NodeNumber;
    int *Node;
    double *KReduced;
    double *FReduced;
    double *UReduced;
    double *L_LU;
    double *U_LU;
    double *Stress;
    double *U;
    double *K;
    double *F;
    //Initial values for element components
    int ElementNumber;
    int *Element;
    //include material, so nx3
public:
    FEMModel();
    ~FEMModel();
    bool ReadTXTFiles(const char *inputName);
    void print(void); //not used in future
    void LUDecomposition(void);

    /* bool ReadKFiles(const char *inputName);  //not used in future */
    /* bool ReadFFiles(const char *inputName);  //not used in future */
    /* void Lprint(void); //not used in future */
    /* void Uprint(void); //not used in future */
    void Assemble(int CarWeight, double Carposition);
    void SolverU(void);
    int Position(const int l, int i, int j);
    void SolverStress(void);
};

#endif /* FEMMODEL_H */
