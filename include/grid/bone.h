#pragma once

const int n = 52, m = 52;

const int obs[][2] = {
{1, 4}, 
{1, 5}, 
{1, 6}, 
{1, 7}, 
{1, 11}, 
{1, 12}, 
{1, 13}, 
{1, 14}, 
{1, 18}, 
{1, 19}, 
{1, 20}, 
{1, 21}, 
{1, 25}, 
{1, 26}, 
{1, 27}, 
{1, 28}, 
{1, 32}, 
{1, 33}, 
{1, 34}, 
{1, 35}, 
{1, 39}, 
{1, 40}, 
{1, 41}, 
{1, 42}, 
{1, 46}, 
{1, 47}, 
{1, 48}, 
{1, 49}, 
{3, 4}, 
{3, 5}, 
{3, 6}, 
{3, 7}, 
{3, 11}, 
{3, 12}, 
{3, 13}, 
{3, 14}, 
{3, 18}, 
{3, 19}, 
{3, 20}, 
{3, 21}, 
{3, 25}, 
{3, 26}, 
{3, 27}, 
{3, 28}, 
{3, 32}, 
{3, 33}, 
{3, 34}, 
{3, 35}, 
{3, 39}, 
{3, 40}, 
{3, 41}, 
{3, 42}, 
{3, 46}, 
{3, 47}, 
{3, 48}, 
{3, 49}, 
{4, 1}, 
{4, 3}, 
{4, 4}, 
{4, 5}, 
{4, 6}, 
{4, 7}, 
{4, 8}, 
{4, 10}, 
{4, 11}, 
{4, 12}, 
{4, 13}, 
{4, 14}, 
{4, 15}, 
{4, 17}, 
{4, 18}, 
{4, 19}, 
{4, 20}, 
{4, 21}, 
{4, 22}, 
{4, 24}, 
{4, 25}, 
{4, 26}, 
{4, 27}, 
{4, 28}, 
{4, 29}, 
{4, 31}, 
{4, 32}, 
{4, 33}, 
{4, 34}, 
{4, 35}, 
{4, 36}, 
{4, 38}, 
{4, 39}, 
{4, 40}, 
{4, 41}, 
{4, 42}, 
{4, 43}, 
{4, 45}, 
{4, 46}, 
{4, 47}, 
{4, 48}, 
{4, 49}, 
{4, 50}, 
{4, 52}, 
{5, 1}, 
{5, 3}, 
{5, 4}, 
{5, 5}, 
{5, 6}, 
{5, 7}, 
{5, 8}, 
{5, 10}, 
{5, 11}, 
{5, 12}, 
{5, 13}, 
{5, 14}, 
{5, 15}, 
{5, 17}, 
{5, 18}, 
{5, 19}, 
{5, 20}, 
{5, 21}, 
{5, 22}, 
{5, 24}, 
{5, 25}, 
{5, 26}, 
{5, 27}, 
{5, 28}, 
{5, 29}, 
{5, 31}, 
{5, 32}, 
{5, 33}, 
{5, 34}, 
{5, 35}, 
{5, 36}, 
{5, 38}, 
{5, 39}, 
{5, 40}, 
{5, 41}, 
{5, 42}, 
{5, 43}, 
{5, 45}, 
{5, 46}, 
{5, 47}, 
{5, 48}, 
{5, 49}, 
{5, 50}, 
{5, 52}, 
{6, 1}, 
{6, 3}, 
{6, 4}, 
{6, 5}, 
{6, 6}, 
{6, 7}, 
{6, 8}, 
{6, 10}, 
{6, 11}, 
{6, 12}, 
{6, 13}, 
{6, 14}, 
{6, 15}, 
{6, 17}, 
{6, 18}, 
{6, 19}, 
{6, 20}, 
{6, 21}, 
{6, 22}, 
{6, 24}, 
{6, 25}, 
{6, 26}, 
{6, 27}, 
{6, 28}, 
{6, 29}, 
{6, 31}, 
{6, 32}, 
{6, 33}, 
{6, 34}, 
{6, 35}, 
{6, 36}, 
{6, 38}, 
{6, 39}, 
{6, 40}, 
{6, 41}, 
{6, 42}, 
{6, 43}, 
{6, 45}, 
{6, 46}, 
{6, 47}, 
{6, 48}, 
{6, 49}, 
{6, 50}, 
{6, 52}, 
{7, 1}, 
{7, 3}, 
{7, 4}, 
{7, 5}, 
{7, 6}, 
{7, 7}, 
{7, 8}, 
{7, 10}, 
{7, 11}, 
{7, 12}, 
{7, 13}, 
{7, 14}, 
{7, 15}, 
{7, 17}, 
{7, 18}, 
{7, 19}, 
{7, 20}, 
{7, 21}, 
{7, 22}, 
{7, 24}, 
{7, 25}, 
{7, 26}, 
{7, 27}, 
{7, 28}, 
{7, 29}, 
{7, 31}, 
{7, 32}, 
{7, 33}, 
{7, 34}, 
{7, 35}, 
{7, 36}, 
{7, 38}, 
{7, 39}, 
{7, 40}, 
{7, 41}, 
{7, 42}, 
{7, 43}, 
{7, 45}, 
{7, 46}, 
{7, 47}, 
{7, 48}, 
{7, 49}, 
{7, 50}, 
{7, 52}, 
{8, 4}, 
{8, 5}, 
{8, 6}, 
{8, 7}, 
{8, 11}, 
{8, 12}, 
{8, 13}, 
{8, 14}, 
{8, 18}, 
{8, 19}, 
{8, 20}, 
{8, 21}, 
{8, 25}, 
{8, 26}, 
{8, 27}, 
{8, 28}, 
{8, 32}, 
{8, 33}, 
{8, 34}, 
{8, 35}, 
{8, 39}, 
{8, 40}, 
{8, 41}, 
{8, 42}, 
{8, 46}, 
{8, 47}, 
{8, 48}, 
{8, 49}, 
{10, 4}, 
{10, 5}, 
{10, 6}, 
{10, 7}, 
{10, 11}, 
{10, 12}, 
{10, 13}, 
{10, 14}, 
{10, 18}, 
{10, 19}, 
{10, 20}, 
{10, 21}, 
{10, 25}, 
{10, 26}, 
{10, 27}, 
{10, 28}, 
{10, 32}, 
{10, 33}, 
{10, 34}, 
{10, 35}, 
{10, 39}, 
{10, 40}, 
{10, 41}, 
{10, 42}, 
{10, 46}, 
{10, 47}, 
{10, 48}, 
{10, 49}, 
{11, 1}, 
{11, 3}, 
{11, 4}, 
{11, 5}, 
{11, 6}, 
{11, 7}, 
{11, 8}, 
{11, 10}, 
{11, 11}, 
{11, 12}, 
{11, 13}, 
{11, 14}, 
{11, 15}, 
{11, 17}, 
{11, 18}, 
{11, 19}, 
{11, 20}, 
{11, 21}, 
{11, 22}, 
{11, 24}, 
{11, 25}, 
{11, 26}, 
{11, 27}, 
{11, 28}, 
{11, 29}, 
{11, 31}, 
{11, 32}, 
{11, 33}, 
{11, 34}, 
{11, 35}, 
{11, 36}, 
{11, 38}, 
{11, 39}, 
{11, 40}, 
{11, 41}, 
{11, 42}, 
{11, 43}, 
{11, 45}, 
{11, 46}, 
{11, 47}, 
{11, 48}, 
{11, 49}, 
{11, 50}, 
{11, 52}, 
{12, 1}, 
{12, 3}, 
{12, 4}, 
{12, 5}, 
{12, 6}, 
{12, 7}, 
{12, 8}, 
{12, 10}, 
{12, 11}, 
{12, 12}, 
{12, 13}, 
{12, 14}, 
{12, 15}, 
{12, 17}, 
{12, 18}, 
{12, 19}, 
{12, 20}, 
{12, 21}, 
{12, 22}, 
{12, 24}, 
{12, 25}, 
{12, 26}, 
{12, 27}, 
{12, 28}, 
{12, 29}, 
{12, 31}, 
{12, 32}, 
{12, 33}, 
{12, 34}, 
{12, 35}, 
{12, 36}, 
{12, 38}, 
{12, 39}, 
{12, 40}, 
{12, 41}, 
{12, 42}, 
{12, 43}, 
{12, 45}, 
{12, 46}, 
{12, 47}, 
{12, 48}, 
{12, 49}, 
{12, 50}, 
{12, 52}, 
{13, 1}, 
{13, 3}, 
{13, 4}, 
{13, 5}, 
{13, 6}, 
{13, 7}, 
{13, 8}, 
{13, 10}, 
{13, 11}, 
{13, 12}, 
{13, 13}, 
{13, 14}, 
{13, 15}, 
{13, 17}, 
{13, 18}, 
{13, 19}, 
{13, 20}, 
{13, 21}, 
{13, 22}, 
{13, 24}, 
{13, 25}, 
{13, 26}, 
{13, 27}, 
{13, 28}, 
{13, 29}, 
{13, 31}, 
{13, 32}, 
{13, 33}, 
{13, 34}, 
{13, 35}, 
{13, 36}, 
{13, 38}, 
{13, 39}, 
{13, 40}, 
{13, 41}, 
{13, 42}, 
{13, 43}, 
{13, 45}, 
{13, 46}, 
{13, 47}, 
{13, 48}, 
{13, 49}, 
{13, 50}, 
{13, 52}, 
{14, 1}, 
{14, 3}, 
{14, 4}, 
{14, 5}, 
{14, 6}, 
{14, 7}, 
{14, 8}, 
{14, 10}, 
{14, 11}, 
{14, 12}, 
{14, 13}, 
{14, 14}, 
{14, 15}, 
{14, 17}, 
{14, 18}, 
{14, 19}, 
{14, 20}, 
{14, 21}, 
{14, 22}, 
{14, 24}, 
{14, 25}, 
{14, 26}, 
{14, 27}, 
{14, 28}, 
{14, 29}, 
{14, 31}, 
{14, 32}, 
{14, 33}, 
{14, 34}, 
{14, 35}, 
{14, 36}, 
{14, 38}, 
{14, 39}, 
{14, 40}, 
{14, 41}, 
{14, 42}, 
{14, 43}, 
{14, 45}, 
{14, 46}, 
{14, 47}, 
{14, 48}, 
{14, 49}, 
{14, 50}, 
{14, 52}, 
{15, 4}, 
{15, 5}, 
{15, 6}, 
{15, 7}, 
{15, 11}, 
{15, 12}, 
{15, 13}, 
{15, 14}, 
{15, 18}, 
{15, 19}, 
{15, 20}, 
{15, 21}, 
{15, 25}, 
{15, 26}, 
{15, 27}, 
{15, 28}, 
{15, 32}, 
{15, 33}, 
{15, 34}, 
{15, 35}, 
{15, 39}, 
{15, 40}, 
{15, 41}, 
{15, 42}, 
{15, 46}, 
{15, 47}, 
{15, 48}, 
{15, 49}, 
{17, 4}, 
{17, 5}, 
{17, 6}, 
{17, 7}, 
{17, 11}, 
{17, 12}, 
{17, 13}, 
{17, 14}, 
{17, 18}, 
{17, 19}, 
{17, 20}, 
{17, 21}, 
{17, 25}, 
{17, 26}, 
{17, 27}, 
{17, 28}, 
{17, 32}, 
{17, 33}, 
{17, 34}, 
{17, 35}, 
{17, 39}, 
{17, 40}, 
{17, 41}, 
{17, 42}, 
{17, 46}, 
{17, 47}, 
{17, 48}, 
{17, 49}, 
{18, 1}, 
{18, 3}, 
{18, 4}, 
{18, 5}, 
{18, 6}, 
{18, 7}, 
{18, 8}, 
{18, 10}, 
{18, 11}, 
{18, 12}, 
{18, 13}, 
{18, 14}, 
{18, 15}, 
{18, 17}, 
{18, 18}, 
{18, 19}, 
{18, 20}, 
{18, 21}, 
{18, 22}, 
{18, 24}, 
{18, 25}, 
{18, 26}, 
{18, 27}, 
{18, 28}, 
{18, 29}, 
{18, 31}, 
{18, 32}, 
{18, 33}, 
{18, 34}, 
{18, 35}, 
{18, 36}, 
{18, 38}, 
{18, 39}, 
{18, 40}, 
{18, 41}, 
{18, 42}, 
{18, 43}, 
{18, 45}, 
{18, 46}, 
{18, 47}, 
{18, 48}, 
{18, 49}, 
{18, 50}, 
{18, 52}, 
{19, 1}, 
{19, 3}, 
{19, 4}, 
{19, 5}, 
{19, 6}, 
{19, 7}, 
{19, 8}, 
{19, 10}, 
{19, 11}, 
{19, 12}, 
{19, 13}, 
{19, 14}, 
{19, 15}, 
{19, 17}, 
{19, 18}, 
{19, 19}, 
{19, 20}, 
{19, 21}, 
{19, 22}, 
{19, 24}, 
{19, 25}, 
{19, 26}, 
{19, 27}, 
{19, 28}, 
{19, 29}, 
{19, 31}, 
{19, 32}, 
{19, 33}, 
{19, 34}, 
{19, 35}, 
{19, 36}, 
{19, 38}, 
{19, 39}, 
{19, 40}, 
{19, 41}, 
{19, 42}, 
{19, 43}, 
{19, 45}, 
{19, 46}, 
{19, 47}, 
{19, 48}, 
{19, 49}, 
{19, 50}, 
{19, 52}, 
{20, 1}, 
{20, 3}, 
{20, 4}, 
{20, 5}, 
{20, 6}, 
{20, 7}, 
{20, 8}, 
{20, 10}, 
{20, 11}, 
{20, 12}, 
{20, 13}, 
{20, 14}, 
{20, 15}, 
{20, 17}, 
{20, 18}, 
{20, 19}, 
{20, 20}, 
{20, 21}, 
{20, 22}, 
{20, 24}, 
{20, 25}, 
{20, 26}, 
{20, 27}, 
{20, 28}, 
{20, 29}, 
{20, 31}, 
{20, 32}, 
{20, 33}, 
{20, 34}, 
{20, 35}, 
{20, 36}, 
{20, 38}, 
{20, 39}, 
{20, 40}, 
{20, 41}, 
{20, 42}, 
{20, 43}, 
{20, 45}, 
{20, 46}, 
{20, 47}, 
{20, 48}, 
{20, 49}, 
{20, 50}, 
{20, 52}, 
{21, 1}, 
{21, 3}, 
{21, 4}, 
{21, 5}, 
{21, 6}, 
{21, 7}, 
{21, 8}, 
{21, 10}, 
{21, 11}, 
{21, 12}, 
{21, 13}, 
{21, 14}, 
{21, 15}, 
{21, 17}, 
{21, 18}, 
{21, 19}, 
{21, 20}, 
{21, 21}, 
{21, 22}, 
{21, 24}, 
{21, 25}, 
{21, 26}, 
{21, 27}, 
{21, 28}, 
{21, 29}, 
{21, 31}, 
{21, 32}, 
{21, 33}, 
{21, 34}, 
{21, 35}, 
{21, 36}, 
{21, 38}, 
{21, 39}, 
{21, 40}, 
{21, 41}, 
{21, 42}, 
{21, 43}, 
{21, 45}, 
{21, 46}, 
{21, 47}, 
{21, 48}, 
{21, 49}, 
{21, 50}, 
{21, 52}, 
{22, 4}, 
{22, 5}, 
{22, 6}, 
{22, 7}, 
{22, 11}, 
{22, 12}, 
{22, 13}, 
{22, 14}, 
{22, 18}, 
{22, 19}, 
{22, 20}, 
{22, 21}, 
{22, 25}, 
{22, 26}, 
{22, 27}, 
{22, 28}, 
{22, 32}, 
{22, 33}, 
{22, 34}, 
{22, 35}, 
{22, 39}, 
{22, 40}, 
{22, 41}, 
{22, 42}, 
{22, 46}, 
{22, 47}, 
{22, 48}, 
{22, 49}, 
{24, 4}, 
{24, 5}, 
{24, 6}, 
{24, 7}, 
{24, 11}, 
{24, 12}, 
{24, 13}, 
{24, 14}, 
{24, 18}, 
{24, 19}, 
{24, 20}, 
{24, 21}, 
{24, 25}, 
{24, 26}, 
{24, 27}, 
{24, 28}, 
{24, 32}, 
{24, 33}, 
{24, 34}, 
{24, 35}, 
{24, 39}, 
{24, 40}, 
{24, 41}, 
{24, 42}, 
{24, 46}, 
{24, 47}, 
{24, 48}, 
{24, 49}, 
{25, 1}, 
{25, 3}, 
{25, 4}, 
{25, 5}, 
{25, 6}, 
{25, 7}, 
{25, 8}, 
{25, 10}, 
{25, 11}, 
{25, 12}, 
{25, 13}, 
{25, 14}, 
{25, 15}, 
{25, 17}, 
{25, 18}, 
{25, 19}, 
{25, 20}, 
{25, 21}, 
{25, 22}, 
{25, 24}, 
{25, 25}, 
{25, 26}, 
{25, 27}, 
{25, 28}, 
{25, 29}, 
{25, 31}, 
{25, 32}, 
{25, 33}, 
{25, 34}, 
{25, 35}, 
{25, 36}, 
{25, 38}, 
{25, 39}, 
{25, 40}, 
{25, 41}, 
{25, 42}, 
{25, 43}, 
{25, 45}, 
{25, 46}, 
{25, 47}, 
{25, 48}, 
{25, 49}, 
{25, 50}, 
{25, 52}, 
{26, 1}, 
{26, 3}, 
{26, 4}, 
{26, 5}, 
{26, 6}, 
{26, 7}, 
{26, 8}, 
{26, 10}, 
{26, 11}, 
{26, 12}, 
{26, 13}, 
{26, 14}, 
{26, 15}, 
{26, 17}, 
{26, 18}, 
{26, 19}, 
{26, 20}, 
{26, 21}, 
{26, 22}, 
{26, 24}, 
{26, 25}, 
{26, 26}, 
{26, 27}, 
{26, 28}, 
{26, 29}, 
{26, 31}, 
{26, 32}, 
{26, 33}, 
{26, 34}, 
{26, 35}, 
{26, 36}, 
{26, 38}, 
{26, 39}, 
{26, 40}, 
{26, 41}, 
{26, 42}, 
{26, 43}, 
{26, 45}, 
{26, 46}, 
{26, 47}, 
{26, 48}, 
{26, 49}, 
{26, 50}, 
{26, 52}, 
{27, 1}, 
{27, 3}, 
{27, 4}, 
{27, 5}, 
{27, 6}, 
{27, 7}, 
{27, 8}, 
{27, 10}, 
{27, 11}, 
{27, 12}, 
{27, 13}, 
{27, 14}, 
{27, 15}, 
{27, 17}, 
{27, 18}, 
{27, 19}, 
{27, 20}, 
{27, 21}, 
{27, 22}, 
{27, 24}, 
{27, 25}, 
{27, 26}, 
{27, 27}, 
{27, 28}, 
{27, 29}, 
{27, 31}, 
{27, 32}, 
{27, 33}, 
{27, 34}, 
{27, 35}, 
{27, 36}, 
{27, 38}, 
{27, 39}, 
{27, 40}, 
{27, 41}, 
{27, 42}, 
{27, 43}, 
{27, 45}, 
{27, 46}, 
{27, 47}, 
{27, 48}, 
{27, 49}, 
{27, 50}, 
{27, 52}, 
{28, 1}, 
{28, 3}, 
{28, 4}, 
{28, 5}, 
{28, 6}, 
{28, 7}, 
{28, 8}, 
{28, 10}, 
{28, 11}, 
{28, 12}, 
{28, 13}, 
{28, 14}, 
{28, 15}, 
{28, 17}, 
{28, 18}, 
{28, 19}, 
{28, 20}, 
{28, 21}, 
{28, 22}, 
{28, 24}, 
{28, 25}, 
{28, 26}, 
{28, 27}, 
{28, 28}, 
{28, 29}, 
{28, 31}, 
{28, 32}, 
{28, 33}, 
{28, 34}, 
{28, 35}, 
{28, 36}, 
{28, 38}, 
{28, 39}, 
{28, 40}, 
{28, 41}, 
{28, 42}, 
{28, 43}, 
{28, 45}, 
{28, 46}, 
{28, 47}, 
{28, 48}, 
{28, 49}, 
{28, 50}, 
{28, 52}, 
{29, 4}, 
{29, 5}, 
{29, 6}, 
{29, 7}, 
{29, 11}, 
{29, 12}, 
{29, 13}, 
{29, 14}, 
{29, 18}, 
{29, 19}, 
{29, 20}, 
{29, 21}, 
{29, 25}, 
{29, 26}, 
{29, 27}, 
{29, 28}, 
{29, 32}, 
{29, 33}, 
{29, 34}, 
{29, 35}, 
{29, 39}, 
{29, 40}, 
{29, 41}, 
{29, 42}, 
{29, 46}, 
{29, 47}, 
{29, 48}, 
{29, 49}, 
{31, 4}, 
{31, 5}, 
{31, 6}, 
{31, 7}, 
{31, 11}, 
{31, 12}, 
{31, 13}, 
{31, 14}, 
{31, 18}, 
{31, 19}, 
{31, 20}, 
{31, 21}, 
{31, 25}, 
{31, 26}, 
{31, 27}, 
{31, 28}, 
{31, 32}, 
{31, 33}, 
{31, 34}, 
{31, 35}, 
{31, 39}, 
{31, 40}, 
{31, 41}, 
{31, 42}, 
{31, 46}, 
{31, 47}, 
{31, 48}, 
{31, 49}, 
{32, 1}, 
{32, 3}, 
{32, 4}, 
{32, 5}, 
{32, 6}, 
{32, 7}, 
{32, 8}, 
{32, 10}, 
{32, 11}, 
{32, 12}, 
{32, 13}, 
{32, 14}, 
{32, 15}, 
{32, 17}, 
{32, 18}, 
{32, 19}, 
{32, 20}, 
{32, 21}, 
{32, 22}, 
{32, 24}, 
{32, 25}, 
{32, 26}, 
{32, 27}, 
{32, 28}, 
{32, 29}, 
{32, 31}, 
{32, 32}, 
{32, 33}, 
{32, 34}, 
{32, 35}, 
{32, 36}, 
{32, 38}, 
{32, 39}, 
{32, 40}, 
{32, 41}, 
{32, 42}, 
{32, 43}, 
{32, 45}, 
{32, 46}, 
{32, 47}, 
{32, 48}, 
{32, 49}, 
{32, 50}, 
{32, 52}, 
{33, 1}, 
{33, 3}, 
{33, 4}, 
{33, 5}, 
{33, 6}, 
{33, 7}, 
{33, 8}, 
{33, 10}, 
{33, 11}, 
{33, 12}, 
{33, 13}, 
{33, 14}, 
{33, 15}, 
{33, 17}, 
{33, 18}, 
{33, 19}, 
{33, 20}, 
{33, 21}, 
{33, 22}, 
{33, 24}, 
{33, 25}, 
{33, 26}, 
{33, 27}, 
{33, 28}, 
{33, 29}, 
{33, 31}, 
{33, 32}, 
{33, 33}, 
{33, 34}, 
{33, 35}, 
{33, 36}, 
{33, 38}, 
{33, 39}, 
{33, 40}, 
{33, 41}, 
{33, 42}, 
{33, 43}, 
{33, 45}, 
{33, 46}, 
{33, 47}, 
{33, 48}, 
{33, 49}, 
{33, 50}, 
{33, 52}, 
{34, 1}, 
{34, 3}, 
{34, 4}, 
{34, 5}, 
{34, 6}, 
{34, 7}, 
{34, 8}, 
{34, 10}, 
{34, 11}, 
{34, 12}, 
{34, 13}, 
{34, 14}, 
{34, 15}, 
{34, 17}, 
{34, 18}, 
{34, 19}, 
{34, 20}, 
{34, 21}, 
{34, 22}, 
{34, 24}, 
{34, 25}, 
{34, 26}, 
{34, 27}, 
{34, 28}, 
{34, 29}, 
{34, 31}, 
{34, 32}, 
{34, 33}, 
{34, 34}, 
{34, 35}, 
{34, 36}, 
{34, 38}, 
{34, 39}, 
{34, 40}, 
{34, 41}, 
{34, 42}, 
{34, 43}, 
{34, 45}, 
{34, 46}, 
{34, 47}, 
{34, 48}, 
{34, 49}, 
{34, 50}, 
{34, 52}, 
{35, 1}, 
{35, 3}, 
{35, 4}, 
{35, 5}, 
{35, 6}, 
{35, 7}, 
{35, 8}, 
{35, 10}, 
{35, 11}, 
{35, 12}, 
{35, 13}, 
{35, 14}, 
{35, 15}, 
{35, 17}, 
{35, 18}, 
{35, 19}, 
{35, 20}, 
{35, 21}, 
{35, 22}, 
{35, 24}, 
{35, 25}, 
{35, 26}, 
{35, 27}, 
{35, 28}, 
{35, 29}, 
{35, 31}, 
{35, 32}, 
{35, 33}, 
{35, 34}, 
{35, 35}, 
{35, 36}, 
{35, 38}, 
{35, 39}, 
{35, 40}, 
{35, 41}, 
{35, 42}, 
{35, 43}, 
{35, 45}, 
{35, 46}, 
{35, 47}, 
{35, 48}, 
{35, 49}, 
{35, 50}, 
{35, 52}, 
{36, 4}, 
{36, 5}, 
{36, 6}, 
{36, 7}, 
{36, 11}, 
{36, 12}, 
{36, 13}, 
{36, 14}, 
{36, 18}, 
{36, 19}, 
{36, 20}, 
{36, 21}, 
{36, 25}, 
{36, 26}, 
{36, 27}, 
{36, 28}, 
{36, 32}, 
{36, 33}, 
{36, 34}, 
{36, 35}, 
{36, 39}, 
{36, 40}, 
{36, 41}, 
{36, 42}, 
{36, 46}, 
{36, 47}, 
{36, 48}, 
{36, 49}, 
{38, 4}, 
{38, 5}, 
{38, 6}, 
{38, 7}, 
{38, 11}, 
{38, 12}, 
{38, 13}, 
{38, 14}, 
{38, 18}, 
{38, 19}, 
{38, 20}, 
{38, 21}, 
{38, 25}, 
{38, 26}, 
{38, 27}, 
{38, 28}, 
{38, 32}, 
{38, 33}, 
{38, 34}, 
{38, 35}, 
{38, 39}, 
{38, 40}, 
{38, 41}, 
{38, 42}, 
{38, 46}, 
{38, 47}, 
{38, 48}, 
{38, 49}, 
{39, 1}, 
{39, 3}, 
{39, 4}, 
{39, 5}, 
{39, 6}, 
{39, 7}, 
{39, 8}, 
{39, 10}, 
{39, 11}, 
{39, 12}, 
{39, 13}, 
{39, 14}, 
{39, 15}, 
{39, 17}, 
{39, 18}, 
{39, 19}, 
{39, 20}, 
{39, 21}, 
{39, 22}, 
{39, 24}, 
{39, 25}, 
{39, 26}, 
{39, 27}, 
{39, 28}, 
{39, 29}, 
{39, 31}, 
{39, 32}, 
{39, 33}, 
{39, 34}, 
{39, 35}, 
{39, 36}, 
{39, 38}, 
{39, 39}, 
{39, 40}, 
{39, 41}, 
{39, 42}, 
{39, 43}, 
{39, 45}, 
{39, 46}, 
{39, 47}, 
{39, 48}, 
{39, 49}, 
{39, 50}, 
{39, 52}, 
{40, 1}, 
{40, 3}, 
{40, 4}, 
{40, 5}, 
{40, 6}, 
{40, 7}, 
{40, 8}, 
{40, 10}, 
{40, 11}, 
{40, 12}, 
{40, 13}, 
{40, 14}, 
{40, 15}, 
{40, 17}, 
{40, 18}, 
{40, 19}, 
{40, 20}, 
{40, 21}, 
{40, 22}, 
{40, 24}, 
{40, 25}, 
{40, 26}, 
{40, 27}, 
{40, 28}, 
{40, 29}, 
{40, 31}, 
{40, 32}, 
{40, 33}, 
{40, 34}, 
{40, 35}, 
{40, 36}, 
{40, 38}, 
{40, 39}, 
{40, 40}, 
{40, 41}, 
{40, 42}, 
{40, 43}, 
{40, 45}, 
{40, 46}, 
{40, 47}, 
{40, 48}, 
{40, 49}, 
{40, 50}, 
{40, 52}, 
{41, 1}, 
{41, 3}, 
{41, 4}, 
{41, 5}, 
{41, 6}, 
{41, 7}, 
{41, 8}, 
{41, 10}, 
{41, 11}, 
{41, 12}, 
{41, 13}, 
{41, 14}, 
{41, 15}, 
{41, 17}, 
{41, 18}, 
{41, 19}, 
{41, 20}, 
{41, 21}, 
{41, 22}, 
{41, 24}, 
{41, 25}, 
{41, 26}, 
{41, 27}, 
{41, 28}, 
{41, 29}, 
{41, 31}, 
{41, 32}, 
{41, 33}, 
{41, 34}, 
{41, 35}, 
{41, 36}, 
{41, 38}, 
{41, 39}, 
{41, 40}, 
{41, 41}, 
{41, 42}, 
{41, 43}, 
{41, 45}, 
{41, 46}, 
{41, 47}, 
{41, 48}, 
{41, 49}, 
{41, 50}, 
{41, 52}, 
{42, 1}, 
{42, 3}, 
{42, 4}, 
{42, 5}, 
{42, 6}, 
{42, 7}, 
{42, 8}, 
{42, 10}, 
{42, 11}, 
{42, 12}, 
{42, 13}, 
{42, 14}, 
{42, 15}, 
{42, 17}, 
{42, 18}, 
{42, 19}, 
{42, 20}, 
{42, 21}, 
{42, 22}, 
{42, 24}, 
{42, 25}, 
{42, 26}, 
{42, 27}, 
{42, 28}, 
{42, 29}, 
{42, 31}, 
{42, 32}, 
{42, 33}, 
{42, 34}, 
{42, 35}, 
{42, 36}, 
{42, 38}, 
{42, 39}, 
{42, 40}, 
{42, 41}, 
{42, 42}, 
{42, 43}, 
{42, 45}, 
{42, 46}, 
{42, 47}, 
{42, 48}, 
{42, 49}, 
{42, 50}, 
{42, 52}, 
{43, 4}, 
{43, 5}, 
{43, 6}, 
{43, 7}, 
{43, 11}, 
{43, 12}, 
{43, 13}, 
{43, 14}, 
{43, 18}, 
{43, 19}, 
{43, 20}, 
{43, 21}, 
{43, 25}, 
{43, 26}, 
{43, 27}, 
{43, 28}, 
{43, 32}, 
{43, 33}, 
{43, 34}, 
{43, 35}, 
{43, 39}, 
{43, 40}, 
{43, 41}, 
{43, 42}, 
{43, 46}, 
{43, 47}, 
{43, 48}, 
{43, 49}, 
{45, 4}, 
{45, 5}, 
{45, 6}, 
{45, 7}, 
{45, 11}, 
{45, 12}, 
{45, 13}, 
{45, 14}, 
{45, 18}, 
{45, 19}, 
{45, 20}, 
{45, 21}, 
{45, 25}, 
{45, 26}, 
{45, 27}, 
{45, 28}, 
{45, 32}, 
{45, 33}, 
{45, 34}, 
{45, 35}, 
{45, 39}, 
{45, 40}, 
{45, 41}, 
{45, 42}, 
{45, 46}, 
{45, 47}, 
{45, 48}, 
{45, 49}, 
{46, 1}, 
{46, 3}, 
{46, 4}, 
{46, 5}, 
{46, 6}, 
{46, 7}, 
{46, 8}, 
{46, 10}, 
{46, 11}, 
{46, 12}, 
{46, 13}, 
{46, 14}, 
{46, 15}, 
{46, 17}, 
{46, 18}, 
{46, 19}, 
{46, 20}, 
{46, 21}, 
{46, 22}, 
{46, 24}, 
{46, 25}, 
{46, 26}, 
{46, 27}, 
{46, 28}, 
{46, 29}, 
{46, 31}, 
{46, 32}, 
{46, 33}, 
{46, 34}, 
{46, 35}, 
{46, 36}, 
{46, 38}, 
{46, 39}, 
{46, 40}, 
{46, 41}, 
{46, 42}, 
{46, 43}, 
{46, 45}, 
{46, 46}, 
{46, 47}, 
{46, 48}, 
{46, 49}, 
{46, 50}, 
{46, 52}, 
{47, 1}, 
{47, 3}, 
{47, 4}, 
{47, 5}, 
{47, 6}, 
{47, 7}, 
{47, 8}, 
{47, 10}, 
{47, 11}, 
{47, 12}, 
{47, 13}, 
{47, 14}, 
{47, 15}, 
{47, 17}, 
{47, 18}, 
{47, 19}, 
{47, 20}, 
{47, 21}, 
{47, 22}, 
{47, 24}, 
{47, 25}, 
{47, 26}, 
{47, 27}, 
{47, 28}, 
{47, 29}, 
{47, 31}, 
{47, 32}, 
{47, 33}, 
{47, 34}, 
{47, 35}, 
{47, 36}, 
{47, 38}, 
{47, 39}, 
{47, 40}, 
{47, 41}, 
{47, 42}, 
{47, 43}, 
{47, 45}, 
{47, 46}, 
{47, 47}, 
{47, 48}, 
{47, 49}, 
{47, 50}, 
{47, 52}, 
{48, 1}, 
{48, 3}, 
{48, 4}, 
{48, 5}, 
{48, 6}, 
{48, 7}, 
{48, 8}, 
{48, 10}, 
{48, 11}, 
{48, 12}, 
{48, 13}, 
{48, 14}, 
{48, 15}, 
{48, 17}, 
{48, 18}, 
{48, 19}, 
{48, 20}, 
{48, 21}, 
{48, 22}, 
{48, 24}, 
{48, 25}, 
{48, 26}, 
{48, 27}, 
{48, 28}, 
{48, 29}, 
{48, 31}, 
{48, 32}, 
{48, 33}, 
{48, 34}, 
{48, 35}, 
{48, 36}, 
{48, 38}, 
{48, 39}, 
{48, 40}, 
{48, 41}, 
{48, 42}, 
{48, 43}, 
{48, 45}, 
{48, 46}, 
{48, 47}, 
{48, 48}, 
{48, 49}, 
{48, 50}, 
{48, 52}, 
{49, 1}, 
{49, 3}, 
{49, 4}, 
{49, 5}, 
{49, 6}, 
{49, 7}, 
{49, 8}, 
{49, 10}, 
{49, 11}, 
{49, 12}, 
{49, 13}, 
{49, 14}, 
{49, 15}, 
{49, 17}, 
{49, 18}, 
{49, 19}, 
{49, 20}, 
{49, 21}, 
{49, 22}, 
{49, 24}, 
{49, 25}, 
{49, 26}, 
{49, 27}, 
{49, 28}, 
{49, 29}, 
{49, 31}, 
{49, 32}, 
{49, 33}, 
{49, 34}, 
{49, 35}, 
{49, 36}, 
{49, 38}, 
{49, 39}, 
{49, 40}, 
{49, 41}, 
{49, 42}, 
{49, 43}, 
{49, 45}, 
{49, 46}, 
{49, 47}, 
{49, 48}, 
{49, 49}, 
{49, 50}, 
{49, 52}, 
{50, 4}, 
{50, 5}, 
{50, 6}, 
{50, 7}, 
{50, 11}, 
{50, 12}, 
{50, 13}, 
{50, 14}, 
{50, 18}, 
{50, 19}, 
{50, 20}, 
{50, 21}, 
{50, 25}, 
{50, 26}, 
{50, 27}, 
{50, 28}, 
{50, 32}, 
{50, 33}, 
{50, 34}, 
{50, 35}, 
{50, 39}, 
{50, 40}, 
{50, 41}, 
{50, 42}, 
{50, 46}, 
{50, 47}, 
{50, 48}, 
{50, 49}, 
{52, 4}, 
{52, 5}, 
{52, 6}, 
{52, 7}, 
{52, 11}, 
{52, 12}, 
{52, 13}, 
{52, 14}, 
{52, 18}, 
{52, 19}, 
{52, 20}, 
{52, 21}, 
{52, 25}, 
{52, 26}, 
{52, 27}, 
{52, 28}, 
{52, 32}, 
{52, 33}, 
{52, 34}, 
{52, 35}, 
{52, 39}, 
{52, 40}, 
{52, 41}, 
{52, 42}, 
{52, 46}, 
{52, 47}, 
{52, 48}, 
{52, 49}
}