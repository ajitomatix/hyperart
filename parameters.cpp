/***************************************************************************
 *   Copyright (C) 2005 by Dr. Douglas Dunham, Ajit Datar                  *
 *   ddunham@d.umn.edu , data0003@d.umn.edu                                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#include "parameters.h"

Parameters::Parameters(int p, int q)
    : p_(p), q_(q)
{
}


Parameters::~Parameters()
{
}

Exposure Parameters::exposure(int layer, int vertexNum, int pgonNum)
{
    if(0 == layer) { //first layer special case
        if(0 == pgonNum) { //first layer, first pgon
            if(3 == p_) {
                return MINEXPOSURE;
            }
            else if(3 == q_) {
                return MAXEXPOSURE;
            }
            else {
                return MINEXPOSURE;
            }
        }
        else { //first layer, not first pgon
            if(3 == p_) {
                return MAXEXPOSURE;
            }
            else if(3 == q_) {
                return MAXEXPOSURE;
            }
            else {
                return MAXEXPOSURE;
            }
        }
    }
    else { //not first layer
        if(0 == vertexNum && 0 == pgonNum) { //first vertex, first pgon
            if(3 == p_) {
                return MINEXPOSURE;
            }
            else if(3 == q_) {
                return MINEXPOSURE;
            }
            else {
                return MINEXPOSURE;
            }
        }
        else if(0 == vertexNum) { //first vertex, but not first pgon
            if(3 == p_) {
                return MAXEXPOSURE;
            }
            else if(3 == q_) {
                return MINEXPOSURE;
            }
            else {
                return MAXEXPOSURE;
            }
        }
        else if(0 == pgonNum) { //first pgon, but not first vertex
            if(3 == p_) {
                return MINEXPOSURE;
            }
            else if(3 == q_) {
                return MAXEXPOSURE;
            }
            else {
                return MINEXPOSURE;
            }
        }
        else { //neither first vertex nor first pgon
            if(3 == p_) {
                return MAXEXPOSURE;
            }
            else if(3 == q_) {
                return MAXEXPOSURE;
            }
            else {
                return MAXEXPOSURE;
            }
        }
    }
}

int Parameters::pSkip(Exposure e)
{
    switch(e) {
        case MINEXPOSURE :
            if( 3 == p_)
                return 1;
            else if( 3 == q_)
                return 3;
            else
                return 1;
        case MAXEXPOSURE :
            if( 3 == p_)
                return 1;
            else if( 3 == q_)
                return 2;
            else
                return 0;
    }
    throw "Invalid exposure value given to pSkip";
}

int Parameters::qSkip(Exposure e, int vertexNum)
{
    switch(e) {
        case MINEXPOSURE :
            if(vertexNum == 0) { //first vertex
                if( 3 == p_)
                    return -1;
                else if( 3 == q_)
                    return 0;
                else
                    return -1;
            }
            else {
                if( 3 == p_)
                    return -1;
                else if( 3 == q_)
                    return 0;
                else
                    return 0;
            }
        case MAXEXPOSURE :
            if(vertexNum == 0) { //first vertex
                if( 3 == p_)
                    return 0;
                else if( 3 == q_)
                    return 0;
                else
                    return -1;
            }
            else {
                /*if( 3 == p_)
                    return 0;
                else if( 3 == q_)
                    return 0;
                else
                    return 0;*/
                return 0; //return 0 in all the 3 cases commented above
            }
    }
    throw "Invalid exposure value given to qSkip";
}

int Parameters::verticesTodo(Exposure e)
{
    switch(e) {
        case MINEXPOSURE :
            if( 3 == p_)
                return 1;
            else if( 3 == q_)
                return (p_ - 5);
            else
                return (p_ - 3);
        case MAXEXPOSURE :
            if( 3 == p_)
                return 1;
            else if( 3 == q_)
                return (p_ - 4);
            else
                return (p_ - 2);
    }
    throw "Invalid exposure value given to verticesTodo";
}

int Parameters::pgonsTodo(Exposure e, int vertexNum)
{
    switch(e) {
        case MINEXPOSURE :
            if(vertexNum == 0) { //first vertex
                if( 3 == p_)
                    return (q_ - 4);
                else if( 3 == q_)
                    return 1;
                else
                    return (q_ - 3);
            }
            else {
                if( 3 == p_)
                    return (q_ - 4);
                else if( 3 == q_)
                    return 1;
                else
                    return (q_ - 2);
            }
        case MAXEXPOSURE :
            if(vertexNum == 0) { //first vertex
                if( 3 == p_)
                    return (q_ - 3);
                else if( 3 == q_)
                    return 1;
                else
                    return (q_ - 3);
            }
            else {
                if( 3 == p_)
                    return (q_ -3);
                else if( 3 == q_)
                    return 1;
                else
                    return (q_ - 2);
            }
    }
    throw "Invalid exposure value given to pgonsTodo";
}

int ParametersIRP::verticesTodo(Exposure e)
{
    switch(e) {
        case MINEXPOSURE :
                return (p_ - 3);
        case MAXEXPOSURE :
                return (p_ - 2);
    }
    throw "Invalid exposure value given to verticesTodo";
}

int ParametersIRP::pgonsToSkip(Exposure e, int vertexNum)
{
    switch(e) {
        case MINEXPOSURE :
        case MAXEXPOSURE :
            if(vertexNum == 0) { //first vertex
                    return 2;
            }
            else {
                    return 3;
            }
    }
    throw "Invalid exposure value given to pgonsToSkip";
}
