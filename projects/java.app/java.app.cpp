//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core\pch.hpp>
#include <core\noncopyable.hpp>
#include <core\factory.hpp>
#include <core\singleton.hpp>
#include <core\status.hpp>
#include <core\unicode.hpp>
#include <core\text.hpp>
#include <core\domain_helper.hpp>
#include <core\logger.hpp>
#include <core\data_provider.hpp>
#include <core\file_data_provider.hpp>
#include <core\content.hpp>
#include <core\counter.hpp>
#include <core\enum.hpp>
#include <core\flags.hpp>
#include <core\tree.hpp>

#include <frontend\grammar\token.hpp>

#include <frontend\fsa\fsa_transition.hpp>
#include <frontend\fsa\fsa_state.hpp>
#include <frontend\fsa\fsa.hpp>
#include <frontend\fsa\fsa_re.hpp>
#include <frontend\fsa\fsa_codegen.hpp>
#include <frontend\fsa\fsa_visualization.hpp>

#include <frontend\lexical_analyzer\lexical_content.hpp>
#include <frontend\lexical_analyzer\lexical_analyzer.hpp>

#include <java\frontend\grammar\java_token.hpp>

#include <java\frontend\lexical_analyzer\java_lexical_analyzer.hpp>

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(java)

int main()
{
    operation_status status; //??

    //if(logger::instance().initialize(LR"(d:\tmp\fe.log)", status))
    {
        //java_unicode_reader_traits::stream_type ufile(java_unicode_reader_traits::open(LR"(D:\Tmp\javaunicodeescapes.txt)"));

        //java_unicode_reader<java_unicode_reader_traits> unicode_reader(ufile);

        //using datum_type = java_lexical_analyzer::datum_type;

        //int32_t count = 0;
        //std::unique_ptr<datum_type> content(new datum_type[16 * 1024]);

        //datum_type codepoint;

        //for(;;)
        //{
        //    codepoint = unicode_reader.read_codepoint();

        //    if(codepoint == java_unicode_reader<java_unicode_reader_traits>::traits::eos_codepoint)
        //    {
        //        break;
        //    }

        //    content.get()[count++] = codepoint;
        //}

        //auto ustr = UnicodeString::fromUTF32(reinterpret_cast<const UChar32*>(content.get()), count);
        //UFILE* cufile(u_fopen_u(reinterpret_cast<const UChar*>(LR"(d:\tmp\content.txt)"), "wb", NULL, "UTF-8"));
        //auto l = ustr.length();
        //        l;
        //auto c = ustr.getCapacity();
        //        c;
        //auto ch = ustr.getBuffer()[l];
        //        ch;
        //bool nil = ustr.getBuffer()[l] == 0;
        //        nil;
        //char16_t* buffer((char16_t*)ustr.getBuffer());
        //content.get()[l] = 0;
        //u_fputs(buffer, cufile);
        //u_fclose(cufile);


        //java_lexical_analyzer::build_numeric_literals(LR"(d:\tmp\java-num-lits.txt)", L"num");
        //java_lexical_analyzer::build_identifiers_and_keywords(LR"(d:\tmp\java-id-keywords.txt)", L"idkws");

        string_type file_path((char_type*)(LR"(d:\tmp\javalexems.txt)"));
        //string_type file_name((char_type*)(LR"(D:\Tmp\java.master.java)"));

        auto total_start = std::chrono::high_resolution_clock::now();

        int k = 0;

        std::filesystem::path path(LR"(D:\Tmp\JAVA.BBBB)");

        for(std::filesystem::directory_iterator it(path), it_end; it != it_end; ++it)
        {
            const std::filesystem::directory_entry& entry(*it);

            string_type file_path = (char_type*)(entry.path().u16string().c_str());
            //string_type file_path = LR"(D:\Tmp\offside.txt)";
            //string_type file_path = LR"(D:\Tmp\JAVA.BBBB\Digits.java)";
            

            java_lexical_analyzer::lexical_analyzer_type lexical_analyzer;

            auto load_start = std::chrono::high_resolution_clock::now();

            java_lexical_analyzer::create(file_path, lexical_analyzer, status);

            auto& lexer(*lexical_analyzer);

            auto load_end = std::chrono::high_resolution_clock::now();

            auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(load_end - load_start);
            std::wcout << L"Loaded content of " << file_path.c_str() << L" in " << microseconds.count() << L" microsecs." << std::endl;

            string_type content_file_path(LR"(D:\Tmp\JAVA.CONTENT\)");
            content_file_path += entry.path().filename();
            content_file_path += L".content.txt";

            UFILE* ucontent(u_fopen_u(reinterpret_cast<const UChar*>(content_file_path.c_str()), "wb", nullptr, "UTF-8"));

            auto scan_start = std::chrono::high_resolution_clock::now();

            for(;;)
            {
                lexer.next_lexeme();
                string_type lname(lexer.lexeme_to_string()); //??
                std::wcout << lname << L"  (" << std::to_wstring((*lexer.content()).get_line_number(lexer.token().offset)) <<
                    L", " << std::to_wstring((*lexer.content()).get_column_number(lexer.token().offset)) << L")" << std::endl;

                //for(int k = 0; k < 48; k++)
                //{
                //    lexer.next_lexeme();
                //    lname = lexer.lexeme_to_string(); //??
                //    std::wcout << lname << L"  (" << std::to_wstring(lexer.get_line_number(lexer.token().offset)) << L", " << std::to_wstring(lexer.get_column_number(lexer.token().offset)) << L")" << std::endl;
                //}

                //{
                //    const auto& la = lexer.lookahead_lexeme();
                //    lname = lexer.lexeme_to_string(la); //??
                //    std::wcout << lname << L"  (" << std::to_wstring(lexer.get_line_number(la.offset)) << L", " << std::to_wstring(lexer.get_column_number(la.offset)) << L")" << std::endl;
                //}
                //{
                //    const auto& la = lexer.lookahead_lexeme();
                //    lname = lexer.lexeme_to_string(la); //??
                //    std::wcout << lname << L"  (" << std::to_wstring(lexer.get_line_number(la.offset)) << L", " << std::to_wstring(lexer.get_column_number(la.offset)) << L")" << std::endl;
                //}
                //{
                //    const auto& la = lexer.lookahead_lexeme();
                //    lname = lexer.lexeme_to_string(la); //??
                //    std::wcout << lname << L"  (" << std::to_wstring(lexer.get_line_number(la.offset)) << L", " << std::to_wstring(lexer.get_column_number(la.offset)) << L")" << std::endl;
                //}
                //{
                //    const auto& la = lexer.lookahead_lexeme();
                //    lname = lexer.lexeme_to_string(la); //??
                //    std::wcout << lname << L"  (" << std::to_wstring(lexer.get_line_number(la.offset)) << L", " << std::to_wstring(lexer.get_column_number(la.offset)) << L")" << std::endl;
                //}


                //auto ustr = UnicodeString::fromUTF32(reinterpret_cast<const UChar32*>(lexer.lexeme_to_codepoints().c_str()), static_cast<int32_t>(lexer.token().length));
                //char16_t* buffer((char16_t*)ustr.getBuffer());
                //buffer[ustr.length()] = 0;
                //string_type c((char_type*)buffer);
                //c += L" [" + lexer.token().name() + L"]:" + std::to_wstring(lexer.token().length);
                //u_fputs((UChar*)c.c_str(), ucontent);

                //if(lexer.token().type == java_lexical_analyzer::token_type::traits::type::unknown)
                //{
                //    std::wcout << L"ERROR in " << file_path.c_str() << L" stopping ..." << std::endl;
                //    std::wcout << lname << std::endl;
                //    //std::wcout << lname << L"  (" << std::to_wstring(lexer.line()) << L", " << std::to_wstring(max(0, (int)(lexer.column() - lexer.token().length))) << L")" << std::endl;
                //    //u_fflush(ucontent);
                //    //u_fclose(ucontent);
                //    //goto _exit;
                //}

                if(lexer.is_eos())
                {
                    break;
                }
            }

            auto scan_end = std::chrono::high_resolution_clock::now();

            microseconds = std::chrono::duration_cast<std::chrono::microseconds>(scan_end - scan_start);
            std::wcout << L"Scanned content of " << file_path.c_str() << L" in " << microseconds.count() << L" microsecs." << std::endl;

            u_fflush(ucontent);
            u_fclose(ucontent);

            if(k > 0 && k++ % 10000)
            {
                std::wcout << L"ITERATION: " << k << std::endl;
            }
        }
//_exit:
        ;

        auto total_end = std::chrono::high_resolution_clock::now();

        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(total_end - total_start);
        std::wcout << L"Total time is " << microseconds.count() << L" , microsecs." << std::endl;
    }

    return 0;
}

// Working directory:  $(SolutionDir)..\build\$(PlatformTarget)\$(Configuration)\
// del /F /Q D:\Tmp\JAVA.CONTENT\*
// java.app.exe > java.lexer.test.txt
