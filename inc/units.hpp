//header file for dealing with users using anything besides MKS units. 
//the bulk of this file is tables to convert to meter
#include <string>
#include <map>
#include <tuple>
namespace unit{   
    namespace tables{
        //stores a conversion table from the short unit code to full name, and to meters
        std::map<char, std::string> code_str{
            //ISO units
            {'ym',"yoctometers"},
            {'zm',"zeptometers"},
            {'am',"attometers"},
            {'fm',"femtometers"},
            {'pm',"picometers"},
            {'nm',"nanometers"},
            {'um',"micrometers"},
            {'mm',"millimeters"},
            {'cm',"centimeters"},
            {'dm',"decimeters"},
            {'m', "meters"},
            {'Dm', "dekameter"},
            {'Hm', "hectometer"},
            {'km', "kilometers"},
            {'Mm',"megameters"},
            {'Gm',"gigameters"},
            {'Tm',"terameters"},
            {'Pm',"petameters"},
            {'Em',"exameters"},
            {'Zm',"zetameters"},
            {'Ym',"yoattameters"},
            //"international"
            {'p',"point"},
            {'P',"pica"},
            {'in',"inch"}, //! get more accurate value
            {'ft', "feet"},
            {'yd',"yards"},
            {'mi',"mile"},
            //US Survey
            {'li',"link"},
            {'fts',"survey foot"}, //this is off by a good bit
            {'rd',"rod"},
            {'ch',"chain"},
            {'fur',"furlong"},
            {'mis',"survey / statute mile"}, //incorrect
            {'lea',"league"},
            //international nautical
            {'ftm',"fathom"},
            {'cbb',"british cable"}, //british cable
            {'cbu',"U.S. cable"},
            {'NMi',"international nautical mile"},
            {'NMu',"U.S. nautical mile"},
            {'NMk', "U.K. nautical mile"},
        };
        std::map<char, double> code_conv{
            {'ym',1E24},
            {'zm',1E21},
            {'am',1E18},
            {'fm',1E15},
            {'pm',1E12},
            {'nm',1E9},
            {'um',1E6},
            {'mm',1E3},
            {'cm',1E2},
            {'dm',1E1},
            {'m',1.0f},
            {'Dm',1E-1},
            {'Hm',1E-2},
            {'km',1E-3},
            {'Mm',1E-6},
            {'Gm',1E-9},
            {'Tm',1E-12},
            {'Pm',1E-15},
            {'Em',1E-18},
            {'Zm',1E-21},
            {'Ym',1E-24},
            //"international"
            {'p',3.5277777777E-4},
            {'P',0.0042175176},
            {'in',0.0254}, //! get more accurate value
            {'ft',0.3048},
            {'yd',1.0936132983},
            {'mi',1609.344},
            //US Survey
            {'li',0.201168},
            {'fts',1}, //this is off by a good bit
            {'rd',0.1988387815},
            {'ch',0.0497096954},
            {'fur',0.0049709695},
            {'mis',1}, //incorrect
            {'lea',0.0002071237},
            //international nautical
            {'ftm',0.54680664916885},
            {'cbb',0.0053961182483768}, //british cable
            {'cbu',0.0045567220764071},
            {'NMi',0.00053995680345572},
            {'NMu',0.00053995680345572},
            {'NMk',0.00053961182483768},
        };
    } // namespace tables
    struct UNFloat{
        float v;
        char UnitCode; // capitalization is important
    };
    std::string getUnitName(UNFloat val){
        return tables::code_str.at(val.UnitCode); //lookup from name table
    }
    //convert tuples to numbers with units
    UNFloat fromTuple(std::tuple<float,char> val){
        UNFloat q; 
        q.v = std::get<float>(val);
        q.UnitCode = std::get<char>(val);
        return q;
    }
    //attaches a unit to a float
    UNFloat fromCLI(float val, char unit){
        UNFloat q;
        q.v = val;
        q.UnitCode = unit;
        return q;
    }
    //convert any supported unit to MKS units
    UNFloat toMKS(UNFloat val){
        val.v = val.v * tables::code_conv.at(val.UnitCode); // convert to meters
        val.UnitCode = 'm'; // update unit
        return val;
    }
} // namespace unit
