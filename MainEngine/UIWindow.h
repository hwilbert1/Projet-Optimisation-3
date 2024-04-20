
#ifndef UIWINDOW_CLASS_H
#define UIWINDOW_CLASS_H


#include "../GraphicEngine/ImGui/imgui.h"
#include "../GraphicEngine/ImGui/imgui_impl_glfw.h"
#include "../GraphicEngine/ImGui/imgui_impl_opengl3.h"
#include <map>
#include <vector>
#include <iostream>
#include "MainEngine.h"
#include <fstream>

class UIWindow
{
public:
	virtual void Display() 
	{
		ImGui::Begin(name.c_str());

		ImGui::End();
	}

	UIWindow(std::string _name)
	{
		name = _name;
	}
protected:

	std::string name = "default";
};





class UIWindowMain : public UIWindow
{
public:
    static void HelpMarker(const char* desc)
    {
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    float GetElevAv(int DebitTotal)
    {

        return 0.007 * DebitTotal + 99.9;
    }

    float GetP1(float ChuteNette, int DebitEntrant)
    {
        double outs = 20.316 - 0.12 * DebitEntrant - 0.588 * ChuteNette - 0.0004 * DebitEntrant * DebitEntrant + 0.014 * DebitEntrant * ChuteNette;
        return std::max(outs, 0.0);
    }
    float GetP2(float ChuteNette, int DebitEntrant)
    {
        double outs = 16.1 - 0.223 * DebitEntrant - 0.465 * ChuteNette - 0.0001 * DebitEntrant * DebitEntrant + 0.016 * DebitEntrant * ChuteNette;
        return std::max(outs, 0.0);
    }
    float GetP3(float ChuteNette, int DebitEntrant)
    {
        double outs = 10.67 - 0.0009 * DebitEntrant - 0.3090 * ChuteNette - 0.0005 * DebitEntrant * DebitEntrant + 0.0109 * DebitEntrant * ChuteNette;
        return std::max(outs, 0.0);
    }
    float GetP4(float ChuteNette, int DebitEntrant)
    {
        double outs = 19.6112 - 0.4494 * DebitEntrant - 0.5603 * ChuteNette + 0.0049 * DebitEntrant * DebitEntrant + 0.0112 * DebitEntrant * ChuteNette - 0.0000189 * DebitEntrant * DebitEntrant * DebitEntrant + 0.00001476 * DebitEntrant * DebitEntrant * ChuteNette;
        return std::max(outs, 0.0);
    }
    float GetP5(float ChuteNette, int DebitEntrant)
    {
        double outs = 12.601 - 0.046 * DebitEntrant - 0.363 * ChuteNette - 0.0005 * DebitEntrant * DebitEntrant + 0.013 * DebitEntrant * ChuteNette;
        return std::max(outs, 0.0);
    }


    float DebitTotal = 541.69;
    float ElevAm = 137.89; //elevation amont
    float MaxQ1 = 160; //maximum temporaire de Q1
    float MaxQ2 = 160;
    float MaxQ3 = 160;
    float MaxQ4 = 160;
    float MaxQ5 = 160;

    float OptQ1 = 0;
    float OptP1 = 0;
    float OptQ2 = 0;
    float OptP2 = 0;
    float OptQ3 = 0;
    float OptP3 = 0;
    float OptQ4 = 0;
    float OptP4 = 0;
    float OptQ5 = 0;
    float OptP5 = 0;

    float HauteurChuteNette = 0;

    std::vector<float> values = { 0 };

    float ElevationAm = 137;
    float DebitTot = 550;
    float X1 = 160, X2 = 160, X3 = 160, X4 = 160, X5 = 160;


    void ProgrammationDynamique()
    {
        //paramètre en entrée d’un instance : débit à repartir, élévation en amont, limitation eventuelle pour chaque turbine

        DebitTotal = std::max(0.0f, DebitTotal);
        ElevAm = std::max(0.0f, ElevAm);
        MaxQ1 = std::max(0.0f, MaxQ1);
        MaxQ2 = std::max(0.0f, MaxQ2);
        MaxQ3 = std::max(0.0f, MaxQ3);
        MaxQ4 = std::max(0.0f, MaxQ4);
        MaxQ5 = std::max(0.0f, MaxQ5);

        
        MaxQ1 = std::min(160.0f, MaxQ1);
        MaxQ2 = std::min(160.0f, MaxQ2);
        MaxQ3 = std::min(160.0f, MaxQ3);
        MaxQ4 = std::min(160.0f, MaxQ4);
        MaxQ5 = std::min(160.0f, MaxQ5);

        DebitTotal = std::min(MaxQ1 + MaxQ2 + MaxQ3 + MaxQ4 + MaxQ5, DebitTotal);
        

        float ElevAv = 0; //elevation aval
        int Step = 5; //increment de débit
        float ChuteNette = 0;

        ElevAv = GetElevAv(DebitTotal);
        ChuteNette = ElevAm - ElevAv;

        HauteurChuteNette = ChuteNette;

        int Q1 = 0, P1 = 0;

        std::map<int, float> etape5; //debit entrant (etat), puissance produite; pas de variable de décision car sortie = 0;

        std::map<int, std::map<int, float>> etape4; //debit entrant (etat), debit sortant selectionné (variable de decision), puissance produite;
        std::map<int, std::pair<int, float>> maxetape4; //debit entrant, (debit sortant, puissance produite) tel que la puissante produite est maximale pour cet etat

        std::map<int, std::map<int, float>> etape3;
        std::map<int, std::pair<int, float>> maxetape3;

        std::map<int, std::map<int, float>> etape2;
        std::map<int, std::pair<int, float>> maxetape2;

        std::map<int, float> etape1;//debit sortant selectionné (variable de decision), puissance produite; pas de debit entrant car debit entrant = min(DebitTotal, SommeMaximum)
        std::pair<int, float> maxetape1;


        int SommeMaximum = MaxQ5;

        //std::cout << "etape 5\n";
        float maxetat = std::min(DebitTotal, (float)SommeMaximum);
        for (int i = 0; i <= maxetat; i = i + 5)  //i : parcours des etat
        {
            //std::cout << i << " : " << GetP5(ChuteNette, i) << "\n";
            etape5.emplace(i, GetP5(ChuteNette, i));

        }
        // ___________________________________________________________________________________________________________________________________________________
        //std::cout << "\netape 4\n";
        int PrevSommeMaximum = SommeMaximum;
        SommeMaximum += MaxQ4;
        maxetat = std::min(DebitTotal, (float)SommeMaximum);
        for (int i = 0; i <= maxetat; i = i + 5)
        {


            std::map<int, float> possibilities;

            int mini = std::min(MaxQ4, (float)i);
            int minj = std::max(i - PrevSommeMaximum, 0);

            int maxQ = minj;
            if (etape5.find(i - minj) != etape5.end())
            {
            float maxP = GetP4(ChuteNette, minj) + etape5.find(i - minj)->second;

            for (int j = minj; j <= mini; j = j + 5) // j parcours des variables de decision
            {
                //   std::cout << "(" << j << " ";

                float P4temp = GetP4(ChuteNette, j) + etape5.find(i - j)->second; //calcule la puissance resultante de cette variable de decision dans cet etat

                //  std::cout << P4temp << ")\t";
                possibilities.emplace(j, P4temp);
                if (P4temp > maxP)//garde la meilleur puissance
                {
                    maxP = P4temp;
                    maxQ = j;
                }
            }


            //   std::cout << i << " " << maxQ << " , " << maxP << ")\n";

            maxetape4.emplace(i, std::pair<int, float>(maxQ, maxP));
            etape4.emplace(i, possibilities);
        }
        }
        // ___________________________________________________________________________________________________________________________________________________
        //std::cout << "\netape 3\n";
        PrevSommeMaximum = SommeMaximum;
        SommeMaximum += MaxQ3;
        maxetat = std::min(DebitTotal, (float)SommeMaximum);
        for (int i = 0; i <= maxetat; i = i + 5)
        {

            //std::cout << i << " :\t";


            std::map<int, float> possibilities;

            int mini = std::min(MaxQ3, (float)i);
            int minj = std::max(i - PrevSommeMaximum, 0);

            int maxQ = minj;
            if (maxetape4.find(i - minj) != maxetape4.end())
            {
            float maxP = GetP3(ChuteNette, minj) + maxetape4.find(i - minj)->second.second;

            for (int j = minj; j <= mini; j = j + 5) // j parcours des variables de decision
            {
                //  std::cout << "(" << j << " ";

                float P3temp = GetP3(ChuteNette, j) + maxetape4.find(i - j)->second.second; //calcule la puissance resultante de cette variable de decision dans cet etat

                // std::cout << P3temp << ")\t";
                possibilities.emplace(j, P3temp);
                if (P3temp > maxP)//garde la meilleur puissance
                {
                    maxP = P3temp;
                    maxQ = j;
                }
            }


            //std::cout << "\nmax (" << maxQ << " , " << maxP << ")\n";

            maxetape3.emplace(i, std::pair<int, float>(maxQ, maxP));
            etape3.emplace(i, possibilities);
        }
        }
        // ___________________________________________________________________________________________________________________________________________________
        //std::cout << "\netape 2\n";
        PrevSommeMaximum = SommeMaximum;
        SommeMaximum += MaxQ2;
        maxetat = std::min(DebitTotal, (float)SommeMaximum);
        int tempint = 0;
        for (int i = 0; i <= maxetat; i = i + 5)
        {


            //std::cout << i << " |\t";

            std::map<int, float> possibilities;

            int mini = std::min(MaxQ2, (float)i);
            int minj = std::max(i - PrevSommeMaximum, 0);

            int maxQ = minj;
            if (maxetape3.find(i - minj) != maxetape3.end())
            {
                float maxP = GetP2(ChuteNette, minj) + maxetape3.find(i - minj)->second.second;


                for (int j = minj; j <= mini; j = j + 5) // j parcours des variables de decision
                {
                    //  std::cout << "(" << j << " ";

                    float P2temp = GetP2(ChuteNette, j) + maxetape3.find(i - j)->second.second; //calcule la puissance resultante de cette variable de decision dans cet etat

                    // std::cout << P2temp << ")\t";
                    possibilities.emplace(j, P2temp);
                    if (P2temp >= maxP)//garde la meilleur puissance
                    {
                        maxP = P2temp;
                        maxQ = j;
                    }
                }

                tempint = i;
                //std::cout << minj << " " << mini << " max (" << maxQ << " , " << maxP << ")\n";


                maxetape2.emplace(i, std::pair<int, float>(maxQ, maxP));
                etape2.emplace(i, possibilities);
            }

        }
        // ___________________________________________________________________________________________________________________________________________________
        //std::cout << "\netape 1\n";

        PrevSommeMaximum = tempint;
        SommeMaximum += MaxQ1;
        Q1 = std::min(DebitTotal, (float)SommeMaximum);


        int maxQ1 = 0;
        float maxP1 = GetP1(ChuteNette, 0) + maxetape2.find(std::min(DebitTotal, (float)PrevSommeMaximum))->second.second;

        if (MaxQ1 != 0)
        {

            //std::cout << Q1 << " |\t";

            int mini1 = std::min(MaxQ1, (float)Q1);
            int minj1 = std::max(Q1 - PrevSommeMaximum, 0);


            maxQ1 = minj1;
            if (maxetape2.find(Q1 - minj1) != maxetape2.end())
            {

                maxP1 = GetP1(ChuteNette, minj1) + maxetape2.find(Q1 - minj1)->second.second;


                for (int j = minj1; j <= mini1; j = j + 5) // j parcours des variables de decision
                {
                    // std::cout << "(" << j << " ";

                    float P1temp = GetP1(ChuteNette, j) + maxetape2.find(Q1 - j)->second.second; //calcule la puissance resultante de cette variable de decision dans cet etat

                    // std::cout << P1temp << ")\t";
                    etape1.emplace(j, P1temp);
                    if (P1temp > maxP1)//garde la meilleur puissance
                    {
                        maxP1 = P1temp;
                        maxQ1 = j;
                    }
                }
                //std::cout << "\nmax (" << maxQ1 << " , " << maxP1 << ")\n";
                maxetape1 = std::pair<int, float>(maxQ1, maxP1);
            }
        }


        //std::cout << ElevAm << "\n";
        //std::cout << DebitTotal << "\n";

        //std::cout << maxP1 << "\n";
        
        OptQ1 = (int)(maxQ1);
        OptP1 = (int)(GetP1(ChuteNette, maxQ1));

        //std::cout << (int)(maxQ1) << " " << (int)(GetP1(ChuteNette, maxQ1)) << "\n";

        int tempQ = DebitTotal - maxQ1;
        if (MaxQ1 == 0)
        {
            tempQ = std::min(DebitTotal, (float)PrevSommeMaximum);
        }
        OptQ2 = (int)(maxetape2.find(tempQ)->second.first);
        OptP2 = (int)(GetP2(ChuteNette, maxetape2.find(tempQ)->second.first));
        //std::cout << (int)(maxetape2.find(tempQ)->second.first) << " " << (int)(GetP2(ChuteNette, maxetape2.find(tempQ)->second.first)) << "\n";


        tempQ = tempQ - maxetape2.find(tempQ)->second.first;

        OptQ3 = (int)(maxetape3.find(tempQ)->second.first);
        OptP3 = (int)(GetP3(ChuteNette, maxetape3.find(tempQ)->second.first));
        //std::cout << (int)(maxetape3.find(tempQ)->second.first) << " " << (int)(GetP3(ChuteNette, maxetape3.find(tempQ)->second.first)) << "\n";

        tempQ = tempQ - maxetape3.find(tempQ)->second.first;
        
        OptQ4 = (int)(maxetape4.find(tempQ)->second.first);
        OptP4 = (int)(GetP4(ChuteNette, maxetape4.find(tempQ)->second.first));
        //std::cout << (int)(maxetape4.find(tempQ)->second.first) << " " << (int)(GetP4(ChuteNette, maxetape4.find(tempQ)->second.first)) << "\n";

        tempQ = tempQ - maxetape4.find(tempQ)->second.first;

        OptQ5 = (int)(tempQ);
        OptP5 = (int)(GetP5(ChuteNette, tempQ));
        //std::cout << (int)(tempQ) << " " << (int)(GetP5(ChuteNette, tempQ)) << "\n";


    }

    void Nomad()
    {

        DebitTot = std::max(0.0f, DebitTot);
        ElevationAm = std::max(0.0f, ElevationAm);
        X1 = std::max(0.0f, X1);
        X2 = std::max(0.0f, X2);
        X3 = std::max(0.0f, X3);
        X4 = std::max(0.0f, X4);
        X5 = std::max(0.0f, X5);


        X1 = std::min(160.0f, X1);
        X2 = std::min(160.0f, X2);
        X3 = std::min(160.0f, X3);
        X4 = std::min(160.0f, X4);
        X5 = std::min(160.0f, X5);

        DebitTot = std::min(X1 + X2 + X3 + X4 + X5, DebitTot);

        std::fstream myfile;
        std::string line;

        std::string filename = ".\\bin\\param.txt";
             
        std::vector<std::string> content;
        myfile.open(filename);
        if (myfile.fail())
        {
            return;
        }
        while (!myfile.eof())
        {
            std::getline(myfile, line); // Check getline() doc, you can retrieve a line before/after a given string etc.
            content.push_back(line);
        }
        myfile.close();

        std::ofstream Outfile;
        Outfile.open(filename);
        if (Outfile.fail())
        {
            return;
        }

        

        for (auto file_line : content)
        {
            if(file_line.find("X0") != -1)
            {
                Outfile << "X0\t( 0 0 0 0 0 " << ElevationAm << " " << DebitTot << " ) " << "\n";
            }
            else if (file_line.find("LOWER_BOUND") != -1)
            {
                Outfile << "LOWER_BOUND\t( 0 0 0 0 0 " << ElevationAm <<" "<< DebitTot << " ) " << "\n";
            }
            else if (file_line.find("UPPER_BOUND") != -1)
            {
                Outfile << "UPPER_BOUND\t(" << X1 << " " << X2 << " " << X3 << " " << X4 << " " << X5 << " " << ElevationAm << " " << DebitTot << ") " << "\n";
            }
            else
            {
                Outfile << file_line << std::endl;
            }
        }
        Outfile << std::endl;
        Outfile.close();

        system(".\\bin\\nomad.exe .\\bin\\param.txt");


        
    }

    
	void Display()
	{

        static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos );
        ImGui::SetNextWindowSize( viewport->WorkSize );
        static bool main = true;
        ImGui::Begin("Main Window", &main, flags);



        static bool p_open = false;
        ImGui::Checkbox("Programmation Dynamique", &p_open);
        static bool p_open2 = false;
        ImGui::Checkbox("Nomad", &p_open2);
        ImGui::End();

        static bool p_open3 = false;
        if (p_open)
        {
            if (ImGui::Begin("Programmation Dynamique", &p_open))
            {
                ImGui::SeparatorText("Definir les parametres");

                const float   DebitTotal_zero = 0.f;

                float DebitTotal_one = 2000.0f;
                ImGui::DragScalar("Debit Total (m3/s)", ImGuiDataType_Float, &DebitTotal, 0.1f, &DebitTotal_zero, &DebitTotal_one, "%0.1f");

                const float   ElevAm_zero = 0.f, ElevAm_one = 2000.f;
                ImGui::DragScalar("Elevation en Amont (m)", ImGuiDataType_Float, &ElevAm, 0.1f, &ElevAm_zero, &ElevAm_one, "%0.1f");

                ImGui::SeparatorText("Selectionner une limite de debit pour les turbines");

                const float   Q_zero = 0.f, Q_one = 160.f;
                ImGui::DragScalar("Max Q1 (m3/s)", ImGuiDataType_Float, &MaxQ1, 0.1f, &Q_zero, &Q_one, "%0.1f");
                ImGui::DragScalar("Max Q2 (m3/s)", ImGuiDataType_Float, &MaxQ2, 0.1f, &Q_zero, &Q_one, "%0.1f");
                ImGui::DragScalar("Max Q3 (m3/s)", ImGuiDataType_Float, &MaxQ3, 0.1f, &Q_zero, &Q_one, "%0.1f");
                ImGui::DragScalar("Max Q4 (m3/s)", ImGuiDataType_Float, &MaxQ4, 0.1f, &Q_zero, &Q_one, "%0.1f");
                ImGui::DragScalar("Max Q5 (m3/s)", ImGuiDataType_Float, &MaxQ5, 0.1f, &Q_zero, &Q_one, "%0.1f");

                ImGui::SeparatorText("");
                if (ImGui::Button("Optimiser par programmation dynamique"))
                {
                    ProgrammationDynamique();
                    values.push_back(OptP1 + OptP2 + OptP3 + OptP4 + OptP5);
                }

                ImGui::SeparatorText("Puissances et Debits Optimaux");
                if (ImGui::BeginTable("Resultats", 4))
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Q1 (m3/s)");
                    ImGui::TableNextColumn();
                    ImGui::Text("%.1f", OptQ1);
                    ImGui::TableNextColumn();
                    ImGui::Text("P1 (MW)");
                    ImGui::TableNextColumn();
                    ImGui::Text("%.1f", OptP1);

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Q2 (m3/s)");
                    ImGui::TableNextColumn();
                    ImGui::Text("%.1f", OptQ2);
                    ImGui::TableNextColumn();
                    ImGui::Text("P2 (MW)");
                    ImGui::TableNextColumn();
                    ImGui::Text("%.1f", OptP2);

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Q3 (m3/s)");
                    ImGui::TableNextColumn();
                    ImGui::Text("%.1f", OptQ3);
                    ImGui::TableNextColumn();
                    ImGui::Text("P3 (MW)");
                    ImGui::TableNextColumn();
                    ImGui::Text("%.1f", OptP3);

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Q4 (m3/s)");
                    ImGui::TableNextColumn();
                    ImGui::Text("%.1f", OptQ4);
                    ImGui::TableNextColumn();
                    ImGui::Text("P4 (MW)");
                    ImGui::TableNextColumn();
                    ImGui::Text("%.1f", OptP4);

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Q5 (m3/s)");
                    ImGui::TableNextColumn();
                    ImGui::Text("%.1f", OptQ5);
                    ImGui::TableNextColumn();
                    ImGui::Text("P5 (MW)");
                    ImGui::TableNextColumn();
                    ImGui::Text("%.1f", OptP5);

                    ImGui::EndTable();
                }

                ImGui::Text("Puissance optimale (MW) : %.1f", OptP1 + OptP2 + OptP3 + OptP4 + OptP5);
                ImGui::Text("Hauteur de chute nette (m) : %.1f", HauteurChuteNette);



                ImGui::SeparatorText("");
               

                ImGui::Checkbox("Afficher le graphe", &p_open3);
                

                ImGui::End();
               
            }
        }

        if (p_open3)
        {
            if (ImGui::Begin("Graphe", &p_open3))
            {
                if (ImGui::Button("Effacer les valeurs"))
                {
                    values.clear();
                    values.push_back(0);
                }
                float* arr = values.data();
                //static float arr[values.size()];
                //std::copy(values.begin(), values.end(), arr);

                //static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };

                HelpMarker(
                    "Ce tableau contient la puissance optimale calculer précédement. L'axe verticale est entre 150 MW et 250 MW\n");
                ImGui::PlotHistogram("Resultats precedents", arr, values.size(), 0, NULL, 150.0f, 250.0f, ImVec2(0, 200.0f));
                
                ImGui::End();
            }
        }

        if (p_open2)
        {
            if (ImGui::Begin("Nomad", &p_open2))
            {
                ImGui::SeparatorText("Definir les parametres");
                const float   DebitTotal_zero = 0.f;

                float DebitTotal_one = 2000.0f;
                ImGui::DragScalar("Debit Total (m3/s)", ImGuiDataType_Float, &DebitTot, 0.1f, &DebitTotal_zero, &DebitTotal_one, "%0.1f");

                const float   ElevAm_zero = 0.f, ElevAm_one = 2000.f;
                ImGui::DragScalar("Elevation en Amont (m)", ImGuiDataType_Float, &ElevationAm, 0.1f, &ElevAm_zero, &ElevAm_one, "%0.1f");

                ImGui::SeparatorText("Selectionner une limite de debit pour les turbines");

                const float   Q_zero = 0.f, Q_one = 160.f;
                ImGui::DragScalar("Max Q1 (m3/s)", ImGuiDataType_Float, &X1, 0.1f, &Q_zero, &Q_one, "%0.1f");
                ImGui::DragScalar("Max Q2 (m3/s)", ImGuiDataType_Float, &X2, 0.1f, &Q_zero, &Q_one, "%0.1f");
                ImGui::DragScalar("Max Q3 (m3/s)", ImGuiDataType_Float, &X3, 0.1f, &Q_zero, &Q_one, "%0.1f");
                ImGui::DragScalar("Max Q4 (m3/s)", ImGuiDataType_Float, &X4, 0.1f, &Q_zero, &Q_one, "%0.1f");
                ImGui::DragScalar("Max Q5 (m3/s)", ImGuiDataType_Float, &X5, 0.1f, &Q_zero, &Q_one, "%0.1f");

                ImGui::SeparatorText("");

                HelpMarker(
                    "Ce boutton va lancer Nomad dans un terminal\nComme il s'agit de problème de maximisation, la fonction objectif a ete inverse car Nomad ne peut traiter que des problemes de minimisation\n");
                if (ImGui::Button("Demarrer Nomad"))
                {

                    Nomad();

                }
                ImGui::End();
            }
        }

		
	}


	UIWindowMain(std::string _name) : UIWindow(_name) {};
protected:
};



class UIWindowControler
{
private:

    static UIWindowControler* _uiwindowcontroler;
public:
    static UIWindowControler* GetInstance()
    {
        if (_uiwindowcontroler == nullptr)
        {
            _uiwindowcontroler = new UIWindowControler();
        }

        return _uiwindowcontroler;
    }

    ~UIWindowControler() {};

    UIWindowControler() {};


	std::map<std::string, UIWindow*> Windows;
	

    void DisplayWindows()
    {
		std::map<std::string, UIWindow*>::iterator it = Windows.begin();
		
		while (it != Windows.end())
		{
			it->second->Display();
			++it;
		}

    };
};
#endif;