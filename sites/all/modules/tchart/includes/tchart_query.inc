<?php
/**
 * Created by PhpStorm.
 * User: RachelWaffle
 * Date: 16/2/16
 * Time: 下午12:20
 */




function queryss($term){

    $category = variable_get('','');
    $settings= array();
    switch($category){
        case 'rainwater_utilization':
            $settings[]=rainwater_utilization($term->tid);
            break;
    }

    return rainwater_utilization($term->tid);
}

/**
 * 雨水资源利用率
 */
function rainwater_utilization($tid){

    //雨水资源利用率
    $query = db_select('rainwater_utilization','n');
    $query->fields('n')->condition('t_id',$tid)->orderBy('samplingtime','ASC');
    $data = $query->execute();

    $category =array();
    $municipal_consumption = array();
    $rainwater_collection_consumption = array();
    $rainwater_utilization =array();
    foreach($data as $entry){
        $category[]= date("Y-m-d",strtotime($entry->samplingtime));
        $municipal_consumption[]=$entry->municipal_consumption/10000;
        $rainwater_collection_consumption[]= $entry->rainwater_collection_consumption/10000;
        $rainwater_utilization[]=$entry->rainwater_utilization*100;
    }

    $query_data[] = $municipal_consumption;
    $query_data[] = $rainwater_collection_consumption;
    $query_data[] = $rainwater_utilization;

    $legend =array('市政杂用水量', '雨水收集利用量','雨水资源利用率');
    $series = build_series($query_data,$legend,array('bar','bar','line'),array(0,0,1));
    $yAxis = build_yAxis(array(array('水量','万立方米'),array('百分比','%')));
    $settings[]=build_settings($series,$legend,$category,$yAxis);
//    $settings[]=array(
//        'chartType' => 'line',
//        'useType' => array('line', 'bar'),
//        'containerId' =>  'line_chart',
//        'options'=>array(
//            'tooltip' => array('trigger' => 'axis'),
//            'legend' => array(
//                'data' => array('市政杂用水量', '雨水收集利用量','雨水资源利用率')
//            ),
//            'toolbox' => array(
//                'show' => TRUE,
//                'feature' => array(
//                    'mark' => array('show' => TRUE),
//                    'dataView' => array('show' => TRUE, 'readOnly' => false),
//                    'magicType' => array(
//                        'show' => TRUE,
//                        'type' => array('line', 'bar')
//                    ),
//                    'restore' => array('show' => TRUE),
//                    'saveAsImage' => array('show' => TRUE),
//                )
//            ),
//            'calculable'=>TRUE,
//            'xAxis' => array(
//                '0' => array(
//                    'type' => 'category',
//                    'data' => $category,
//                )
//            ),
//            'yAxis' => array(
//                '0' => array(
//                    'type' => 'value',
//                    'name' =>'水量',
//                    'axisLabel'=>array('formatter'=>'{value}万立方米'),
//                    'splitArea' => TRUE
//                ),
//                '1' => array(
//                    'type' => 'value',
//                    'name' =>'百分比',
//                    'axisLabel'=>array('formatter'=>'{value}%'),
//                    'splitArea' => TRUE
//                ),
//            ),
//            'series' => array(
//                '0' => array(
//                    'name' => "市政杂用水量",
//                    'type' => 'bar',
//                    'data' => $municipal_consumption,
//                ),
//                '1' => array(
//                    'name' => "雨水收集利用量",
//                    'type' => 'bar',
//                    'data' => $rainwater_collection_consumption,
//                ),
//                '2'=>array(
//                    'name' => "雨水资源利用率",
//                    'type' => 'line',
//                    'yAxisIndex'=>1,
//                    'data' => $rainwater_utilization,
//                ),
//            ),
//
//        ),
//
//    );
    return $settings;
}

/**
 * 面源污染数据
 */
function npsp_data($tid){
    $query = db_select('NPSP_data','n');
    $query->fields('n')->condition('t_id',$tid)->orderBy('samplingtime','ASC');
    $data = $query->execute();

    $category =array();
    $ph = array();
    $do = array();
    $ss = array();
    $cod = array();
    $nh3_n = array();
    $tn = array();
    $tp = array();
    foreach($data as $entry){
        $category[]= date("Y-m-d",strtotime($entry->samplingtime));
        //PH
        $ph[] = $entry->PH;
        //DO
        $do[] = $entry->DO;
        //SS
        $ss[] = $entry->SS;
        //COD
        $cod[] = $entry->COD;
        //NH3-N
        $nh3_n[] = $entry->NH3_N;
        //TN
        $tn[] = $entry->TN;
        //TP
        $tp[] = $entry->TP;
    }

    $query_data_po[]= $ss;
    $query_data_po[]= $cod;
    $query_data_po[]= $nh3_n;
    $query_data_po[]= $tn;
    $query_data_po[]= $tp;
    $legend_po=array('SS浓度', 'COD浓度','氨氮浓度','TN浓度','TP浓度');
    $series_po= build_series($query_data_po,$legend_po,build_type(5,'bar'));
    $yAxis_po=build_yAxis(array(array('污染物浓度','mg/L')));
    $settings[]= build_settings($series_po,$legend_po,$category,$yAxis_po);

    $query_data_ph[]=$ph;
    $legend_ph = array('PH');
    $series_ph = build_series($query_data_ph,$legend_ph,build_type(1,'bar'));
    $yAxis_ph = build_yAxis(array(array('PH值',' ')));
    $settings[] = build_settings($series_ph,$legend_ph,$category,$yAxis_ph);
    return $settings;

    $settings[]=array(
        'chartType' => 'line',
        'useType' => array('line', 'bar'),
        'containerId' =>  'line_chart',
        'options'=>array(
            'tooltip' => array('trigger' => 'axis'),
            'legend' => array(
                'data' => array('SS浓度', 'COD浓度','氨氮浓度','TN浓度','TP浓度')
            ),
            'toolbox' => array(
                'show' => TRUE,
                'feature' => array(
                    'mark' => array('show' => TRUE),
                    'dataView' => array('show' => TRUE, 'readOnly' => false),
                    'magicType' => array(
                        'show' => TRUE,
                        'type' => array('line', 'bar')
                    ),
                    'restore' => array('show' => TRUE),
                    'saveAsImage' => array('show' => TRUE),
                )
            ),
            'calculable'=>TRUE,
            'xAxis' => array(
                '0' => array(
                    'type' => 'category',
                    'data' => $category,
                )
            ),
            'yAxis' => array(
                '0' => array(
                    'type' => 'value',
                    'name' =>'污染浓度',
                    'axisLabel'=>array('formatter'=>'{value}mg/L'),
                    'splitArea' => TRUE
                ),
            ),
            'series' => array(
                '0' => array(
                    'name' => "SS浓度",
                    'type' => 'bar',
                    'data' => $ss,
                ),
                '1' => array(
                    'name' => "COD浓度",
                    'type' => 'bar',
                    'data' => $cod,
                ),
                '2' => array(
                    'name' => "氨氮浓度",
                    'type' => 'bar',
                    'data' => $nh3_n,
                ),
                '3' => array(
                    'name' => "TN浓度",
                    'type' => 'bar',
                    'data' => $tn,
                ),
                '4' => array(
                    'name' => "TP浓度",
                    'type' => 'bar',
                    'data' => $tp,
                ),

            ),

        ),

    );

    $settings[]=array(
        'chartType' => 'line',
        'useType' => array('line', 'bar'),
        'containerId' =>  'line_chart',
        'options'=>array(
            'tooltip' => array('trigger' => 'axis'),
            'legend' => array(
                'data' => array('PH值')
            ),
            'toolbox' => array(
                'show' => TRUE,
                'feature' => array(
                    'mark' => array('show' => TRUE),
                    'dataView' => array('show' => TRUE, 'readOnly' => false),
                    'magicType' => array(
                        'show' => TRUE,
                        'type' => array('line', 'bar')
                    ),
                    'restore' => array('show' => TRUE),
                    'saveAsImage' => array('show' => TRUE),
                )
            ),
            'calculable'=>TRUE,
            'xAxis' => array(
                '0' => array(
                    'type' => 'category',
                    'data' => $category,
                )
            ),
            'yAxis' => array(
                '0' => array(
                    'type' => 'value',
                    'name' =>'PH值',
                    'axisLabel'=>array('formatter'=>'{value} '),
                    'splitArea' => TRUE
                ),
            ),
            'series' => array(
                '0' => array(
                    'name' => "PH值",
                    'type' => 'bar',
                    'data' => $ph,
                ),
            ),

        ),

    );

    return $settings;
}

/*
 * 地下水位
 */
function groundwaterlevel($tid,$name,$containerId=-1)
{
    //地下水位
    $query = db_select('groundwaterlevel', 'n');
    $query->fields('n')->condition('t_id', $tid)->orderBy('samplingtime', 'ASC');
    $data = $query->execute();

    $category = array();
    $grandwaterlevel = array();
    foreach ($data as $entry) {
        $category[] = date("Y-m-d", strtotime($entry->samplingtime));
        $grandwaterlevel[] = $entry->$grandwaterlevel;
    }

    $query_data[] = $grandwaterlevel;

    $legend = array($name);
    $series = build_series($query_data, $legend, array('line'));
    $yAxis = build_yAxis(array(array('地下水深', 'cm')));
    $settings[] = build_settings($series, $legend, $category, $yAxis, $containerId);
    return $settings;
}

/**
 * 管网漏损控制率
 */
function pipe_leakage_rate($tid){
    $query = db_select('pipe_leakage_rate','n');
    $query->fields('n')->condition('t_id',$tid)->orderBy('samplingtime','ASC');
    $data = $query->execute();

    $category =array();
    $total_water_supply = array();
    $actual_usage = array();
    $pipe_leakage_rate =array();
    foreach($data as $entry){
        $category[]= date("Y-m-d",strtotime($entry->samplingtime));
        $total_water_supply[]=$entry->total_water_supply;
        $actual_usage[]= $entry->actual_usage;
        $pipe_leakage_rate[]=$entry->pipe_leakage_rate*100;
    }

    $query_data[] = $total_water_supply;
    $query_data[] = $actual_usage;
    $query_data[] = $pipe_leakage_rate;

    $legend =array('总供水量', '实际使用量','管网控制率');
    $series = build_series($query_data,$legend,array('bar','bar','line'),array(0,0,1));
    $yAxis = build_yAxis(array(array('水量','万立方米'),array('百分比','%')));
    $settings[]=build_settings($series,$legend,$category,$yAxis);
    return $settings;
}

/**
 * 污水再生利用率
 */
function wastewater_reuse_rate($tid){
    $query = db_select('wastewater_reuse_rate','n');
    $query->fields('n')->condition('t_id',$tid)->orderBy('samplingtime','ASC');
    $data = $query->execute();

    $category =array();
    $sewage_treat_capacity = array();
    $reuse_capacity = array();
    $ecological_capacity = array();
    $reuse_rate =array();
    foreach($data as $entry){
        $category[]= date("Y-m-d",strtotime($entry->samplingtime));
        $sewage_treat_capacity[]=$entry->sewage_treat_capacity;
        $reuse_capacity[]= $entry->reuse_capacity;
        $ecological_capacity[]= $entry->ecological_capacity;
        $reuse_rate[]=$entry->reuse_rate*100;
    }

    $query_data[] = $sewage_treat_capacity;
    $query_data[] = $reuse_capacity;
    $query_data[] = $ecological_capacity;
    $query_data[] = $reuse_rate;

    $legend =array('污水处理量', '再生回用水量','生态处理水量','污水再生利用率');
    $series = build_series($query_data,$legend,array('bar','bar','bar','line'),array(0,0,0,1));
    $yAxis = build_yAxis(array(array('水量','万立方米'),array('百分比','%')));
    $settings[]=build_settings($series,$legend,$category,$yAxis);
    return $settings;
}



function build_settings($series,$legend,$category,$yAxis,$containerId=-1){

    $settings=array(
        'chartType' => 'line',
        'useType' => array('line', 'bar'),
//        'containerId' =>  'line_chart',
        'options'=>array(
            'tooltip' => array('trigger' => 'axis'),
            'legend' => array(
                'data' => $legend,
            ),
            'toolbox' => array(
                'show' => TRUE,
                'feature' => array(
                    'mark' => array('show' => TRUE),
                    'dataView' => array('show' => TRUE, 'readOnly' => false),
                    'magicType' => array(
                        'show' => TRUE,
                        'type' => array('line', 'bar')
                    ),
                    'restore' => array('show' => TRUE),
                    'saveAsImage' => array('show' => TRUE),
                )
            ),
            'calculable'=>TRUE,
            'xAxis' => array(
                '0' => array(
                    'type' => 'category',
                    'data' => $category,
                )
            ),
            'yAxis' => $yAxis,
            'series' => $series,
        ),

    );
    if($containerId!=-1){
        $settings+=array('containerId'=>$containerId);
    }

    return $settings;
}


function build_series($data,$name,$type,$yAxisIndex=0){


    $series = array();
    $count = 0;
    foreach($data as $item){
        $entry = array(
            'name'=>$name[$count],
            'type'=>$type[$count],
            'data'=>$item,
        );
        if(is_array($yAxisIndex)){
            $entry+=array('yAxisIndex'=>$yAxisIndex[$count]);
        }else{
            $entry+=array('yAxisIndex'=>$yAxisIndex);
        }
        $series[] = $entry;
        $count++;
    }
    return $series;
}

function build_type($num,$type){
    $type_arr= array();
    for($i = 0;$i<$num;$i++){
        $type_arr[]= $type;
    }
    return $type_arr;
}

function build_yAxis($name_label){
    $yAxis =array();
    foreach($name_label as $item){
        $yAxis[]=array(
                'type' => 'value',
                'name' =>$item[0],
                'axisLabel'=>array('formatter'=>'{value}'.$item[1]),
                'splitArea' => TRUE
        );
    }
    return $yAxis;
}