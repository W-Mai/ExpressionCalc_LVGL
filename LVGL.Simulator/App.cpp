#include "App.h"
#include "../lvgl/lvgl.h"

using namespace std;

static set<string> func_set = {
    "sin", "asin","ln","sign",
    "cos", "acos","log","floor",
    "tan", "atan","log2","ceil",
    "abs", "sqrt","sqrt","^"
};

static set<string> const_set = {
    "PI", "E"
};

static vector<string> expression;

static void btn_event_cb(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    auto* obj = lv_event_get_target(e);
    auto* label = static_cast<lv_obj_t*>(lv_event_get_user_data(e));



    if (code == LV_EVENT_VALUE_CHANGED) {
        uint32_t id = lv_btnmatrix_get_selected_btn(obj);
        const char* txt = lv_btnmatrix_get_btn_text(obj, id);

        LV_LOG_USER("%s was pressed\n", txt);

        if (txt == LV_SYMBOL_BACKSPACE) {
            if (!expression.empty()) {
                expression.pop_back();
            }
        } else if (txt == LV_SYMBOL_TRASH) {
            expression.clear();
        } else if (txt == LV_SYMBOL_NEW_LINE) {
            string text;
            for (const auto x : expression) {
                text += x;
            }

            Error e;

            const auto res = reversePolishNotation(text, tokenLevel, e);
            if (e.type != ErrorType::Well) {
                lv_label_set_text(label, ("[" + ErrorType2Name[e.type] + "]: " + e.msg + ".").c_str());
                return;
            }

            const auto val = evalNotation(res, e);
            if (e.type != ErrorType::Well) {
                lv_label_set_text(label, ("[" + ErrorType2Name[e.type] + "]: " + e.msg + ".").c_str());
                return;
            }

            const auto real_res = to_string(val);
            lv_label_set_text(label, real_res.c_str());
            expression.clear();
            expression.push_back(real_res);
            return;
        } else {
            if (const_set.find(txt) != const_set.end()) {
                expression.emplace_back(string(txt) + "()");
            } else if (func_set.find(txt) != func_set.end()) {
                expression.emplace_back(string(txt) + "(");
            } else {
                expression.emplace_back(string(txt));
            }
        }

        string text;
        for (const auto x : expression) {
            text += x;
        }
        if (text.empty()) {
            lv_label_set_text(label, "0");
        }else{
            lv_label_set_text(label, text.c_str());
        }

    }
}

static const char* btnm_number_map[] = {
    "7", "8", "9", "\n",
    "4", "5", "6", "\n",
    "1", "2", "3", "\n",
    "0", ".",""
};
static const char* btnm_ops_map[] = {
    "(", ")", "\n",
    "/", "+", "\n",
    "*", "-", "\n",
    "\\", "%","\n",
    LV_SYMBOL_NEW_LINE,""
};

static const char* btnm_funcs_map[] = {
    "PI","E",LV_SYMBOL_TRASH, LV_SYMBOL_BACKSPACE,"\n",
    "sin", "asin","ln","sign","\n",
    "cos", "acos","log","floor","\n",
    "tan", "atan","log2","ceil","\n",
    "abs", "sqrt","sqrt","^", ""
};


// 0 1 2 3 4 5 6 7 8 9 .
// + - * / % \
// ( )
// abs srqt sin cos tan floor ceil sign ln log10 lg2 x^2 ^


void App() {
    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 0);
    lv_style_set_border_width(&style_btn, 0);
    lv_style_set_border_opa(&style_btn, LV_OPA_50);
    lv_style_set_border_color(&style_btn, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_border_side(&style_btn, LV_BORDER_SIDE_INTERNAL);
    lv_style_set_shadow_width(&style_btn, 0);
    lv_style_set_radius(&style_btn, 0);

    static lv_style_t style_dsp_label = style_btn;

    static lv_style_t style_dsp_text;
    lv_style_init(&style_dsp_text);
    lv_style_set_text_font(&style_dsp_text, &lv_font_montserrat_36);


    auto* dsp_label_cont = lv_obj_create(lv_scr_act());
    auto* dsp_label = lv_label_create(dsp_label_cont);
    //lv_obj_set_style_base_dir(dsp_label, LV_BASE_DIR_RTL, 0);
    lv_label_set_text(dsp_label, "Expression Calculator.");
    lv_obj_set_size(dsp_label_cont, 500, 80);
    lv_obj_align(dsp_label, LV_ALIGN_RIGHT_MID, -5, 0);

    auto* btnm_number = lv_btnmatrix_create(lv_scr_act());
    lv_obj_set_style_radius(btnm_number, 0, 0);
    lv_btnmatrix_set_map(btnm_number, btnm_number_map);
    lv_obj_set_size(btnm_number, 194, 250);
    lv_btnmatrix_set_btn_width(btnm_number, 9, 2);

    auto* btnm_ops = lv_btnmatrix_create(lv_scr_act());
    lv_obj_set_style_radius(btnm_ops, 0, 0);
    lv_btnmatrix_set_map(btnm_ops, btnm_ops_map);
    lv_obj_set_size(btnm_ops, 120, 250);

    lv_obj_add_style(btnm_number, &style_btn, LV_PART_ITEMS);
    lv_obj_add_style(btnm_ops, &style_btn, LV_PART_ITEMS);

    auto* btnm_funcs = lv_btnmatrix_create(lv_scr_act());
    lv_obj_set_style_radius(btnm_funcs, 0, 0);
    lv_btnmatrix_set_map(btnm_funcs, btnm_funcs_map);
    lv_obj_set_size(btnm_funcs, 200, 250);

    lv_obj_add_style(btnm_number, &style_btn, LV_PART_ITEMS);
    lv_obj_add_style(btnm_ops, &style_btn, LV_PART_ITEMS);
    lv_obj_add_style(btnm_funcs, &style_btn, LV_PART_ITEMS);
    lv_obj_add_style(dsp_label_cont, &style_dsp_label, 0);
    lv_obj_add_style(dsp_label, &style_dsp_text, 0);

    lv_obj_align(dsp_label_cont, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_align_to(btnm_funcs, dsp_label_cont, LV_ALIGN_OUT_BOTTOM_LEFT, -10, 10);
    lv_obj_align_to(btnm_number, btnm_funcs, LV_ALIGN_OUT_RIGHT_MID, 3, 0);
    lv_obj_align_to(btnm_ops, btnm_number, LV_ALIGN_OUT_RIGHT_MID, 3, 0);

    lv_obj_add_event_cb(btnm_number, btn_event_cb, LV_EVENT_ALL, dsp_label);
    lv_obj_add_event_cb(btnm_ops, btn_event_cb, LV_EVENT_ALL, dsp_label);
    lv_obj_add_event_cb(btnm_funcs, btn_event_cb, LV_EVENT_ALL, dsp_label);
}

