<?php
/**
 * Created by PhpStorm.
 * User: RachelWaffle
 * Date: 16/2/16
 * Time: 下午12:20
 */

/**
 * 根据配置获取内容
 */
function get_charts($term)
{

    $category = variable_get('tchart_term_db_' . $term->tid, 'rainwater_utilization');

//	print_r(_get_children_tids($term->tid));

    taxonomy_get_children($term->tid);
    $settings = array();
    $drupal_settings = array();
    $tid = $term->tid;
    $output = '';
    switch ($category) {
        case 'rainwater_utilization':
            $settings += rainwater_utilization($tid);
            break;
        case 'pipe_leakage_rate':
            $drupal_settings += pipe_leakage_rate($tid);
            break;
        case 'waterenviroment_quality':
            $drupal_settings += waterenviroment_quality($tid);
            break;
        case 'wastewater_reuse_rate':
            $drupal_settings += wastewater_reuse_rate($tid);
            break;
        case 'npsp_data':
            $drupal_settings += npsp_data($tid);
            break;
        case 'ecologicalbankline_data':
            $settings += ecologicalbankline_data($tid);
            break;
        case 'groundwater_quality':
//            $output .= groundwater_quality($tid);
            break;
        case 'groundwaterlevel':
            $drupal_settings += groundwaterlevel($term);
            break;
        case 'streamflow_control_rate':
            $drupal_settings += streamflow_control_rate($term);
            break;
    }
    if (!empty($settings)) {
        $output = render_echarts(array('echart' => $settings)) . $output;
    }
    return array('output' => $output, 'settings' => $drupal_settings);
}

/**
 * 雨水资源利用率
 */
function rainwater_utilization($tid)
{

    //雨水资源利用率
    $query = db_select('rainwater_utilization', 'n');
    $query->fields('n')->condition('t_id', $tid)->orderBy('samplingtime', 'ASC');
    $data = $query->execute();

    $category = array();
    $municipal_consumption = array();
    $rainwater_collection_consumption = array();
    $rainwater_utilization = array();
    foreach ($data as $entry) {
        $category[] = date("Y-m-d", strtotime($entry->samplingtime));
        $municipal_consumption[] = $entry->municipal_consumption / 10000;
        $rainwater_collection_consumption[] = $entry->rainwater_collection_consumption / 10000;
        $rainwater_utilization[] = $entry->rainwater_utilization * 100;
    }

    $query_data[] = $municipal_consumption;
    $query_data[] = $rainwater_collection_consumption;
    $query_data[] = $rainwater_utilization;

    $legend = array('市政杂用水量', '雨水收集利用量', '雨水资源利用率');
    $series = build_series($query_data, $legend, array(
        'bar',
        'bar',
        'line'
    ), array(0, 0, 1));
    $xAxis = build_category_axis($category);
    $yAxis = build_value_axis(array(array('水量', '万立方米'), array('百分比', '%')));
    $settings[] = build_settings($series, $legend, $xAxis, $yAxis);

    return $settings;
}

/**
 * 面源污染数据
 */
function npsp_data($tid)
{
    $data_settings = array();
    $date_query = db_select('npsp_data', 'n')
        ->fields('n')
        ->condition('t_id', $tid)
        ->orderBy('samplingtime', 'ASE')
        ->execute();

    //一类参数
    //可重构:后台设置
    $category = array(
        0 => array(
            'samplingtime' => array('时间'),
            'SS' => array('SS削减率', '%'),
            'COD' => array('COD削减率', '%'),
            'NH3_N' => array('氨氮削减率', '%'),
            'TN' => array('TN削减率', '%'),
            'TP' => array('TP削减率', '%'),
        ),

    );

    $data_set = array();
    //数据源分类
    foreach ($date_query as $item) {
        $entry = (array)$item;
        $data_set[] = $entry;
        unset($entry['t_id']);
        unset($entry['id']);
        unset($entry['uploadid']);
        $i = 1;
        foreach ($category as $category_1) {
            //添加theme和title
            $chart_name = taxonomy_term_load($item->t_id)->name;
            $chart_drupal = '-' . $chart_name;
            $data_settings[$chart_drupal]['#theme'] = 'tchart_time_chart2';
            $data_settings[$chart_drupal]['#title'] = $chart_name;
            $data_settings[$chart_drupal]['#category'] = $category_1;
            $data_settings[$chart_drupal]['#height'] = 270;
            //添加item
            foreach ($category_1 as $key => $value) {
                $data_settings[$chart_drupal]['#items'][$key][] = $entry[$key];
            }
            $i++;
        }
    }

    return $data_settings;

}

/*
 * 地下水位
 */
function groundwaterlevel($term)
{
    $tid = $term->tid;
    if (variable_get('tchart_show_depth_' . $tid, FALSE)) {
        //添加:获取深度选项

    }


    $tids = array($tid);
    $result = array('legend' => array(), 'settings' => array());
    //地下水位
//    foreach ($tids as $db_tid) {
    $query = db_select('groundwaterlevel', 'n');
    $query->fields('n')
        //读取数据库中符合数据的tid
        ->condition('t_id', array($tids))
        ->orderBy('samplingtime', 'ASC');
    $data = $query->execute();
    $result = groundwaterlevel_chart($data);
    //升序排列legend
    asort($result['legend']);

    if (empty($result['legend'])) {
        return [];
    }
    //表格
    $header = array('名称', '区域', '历史平均水位', '最大值', '最小值', '平均值');
    $table_depth = array();
    foreach ($result['legend'] as $item) {
        $depth_data = $result['settings'][$item]['data'];
        $count = 0;
        $max = $depth_data[0][1];
        $min = $max;
        $total = 0;
        //最大最小平均值
        foreach ($depth_data as $each_depth) {
            if ($max < $each_depth[1]) {
                $max = $each_depth[1];
            }
            if ($min > $each_depth[1]) {
                $min = $each_depth[1];
            }
            $total += $each_depth[1];
            $count++;
        }
        $table_depth[] = array('name' => $item, 'area' => $term->name, 'historicalaverage' => $result['settings'][$item]['average'] . 'm', 'max' => $max . 'm', 'min' => $min . 'm', 'average' => number_format($total / $count, 2) . 'm');
    }
    $chart_set = ['#theme' => 'tchart_report', '#header' => $header, '#items' => $table_depth];

    $set = tchart_build_timechart_array($result['legend'], $result['settings'], '', array(
        'unit' => '深度',
        'per' => 'm'
    ));

    $settings[] = $set;
    $settings[] = $chart_set;
    return $settings;
}

/**
 * 地下水位读取数据库对象数据成为原始数据
 */
function groundwaterlevel_chart($data, $containerId = -1)
{

    $category = array();
    $chart_set = array();
    foreach ($data as $entry) {
        $has_cat = FALSE;
        foreach ($category as $cat) {
            if ($entry->location == $cat) {
                $has_cat = TRUE;
                break;
            }
        }
        if (!$has_cat) {
            $name = $entry->location;
            $category[] = $name;
            $chart_set[$name]['name'] = $name;
            $chart_set[$name]['type'] = 'line';
            $chart_set[$name]['average'] = $entry->historicalaverage;
            $chart_set[$name]['data'] = array();
        }


        foreach ($category as $cat) {

            if ($entry->location == $cat) {
                array_push($chart_set[$cat]['data'], array(
                    strtotime($entry->samplingtime),
                    $entry->groundwaterdepth
                ));
                break;
            }
        }
    }
    return array('legend' => $category, 'settings' => $chart_set);

}

/**
 * 管网漏损控制率
 */
function pipe_leakage_rate($tid)
{
    $data_query = db_select('pipe_leakage_rate', 'n')
        ->fields('n')
        ->condition('t_id', $tid)
        ->orderBy('samplingtime', 'ASE')
        ->execute();

    //三类参数
    $category = array(
        0 => array(
            'samplingtime' => array('时间'),
            'total_water_supply' => array('总供水量', '万m³'),
            'actual_usage' => array('实际利用量', '万m³'),
            'pipe_leakage_rate' => array('管网漏损率', '%', 'line'),
        ),
    );

    $chart_name = ['chart1','chart2'];
    $data_settings = time_chart2_build($category,$data_query,$chart_name);

    $add = count(current($data_settings)['#items']['samplingtime']);

    for($i = 1; $i<$add;$i++){

    }
    $option = [[
        'show_zoom' => FALSE,
        'yAxis_extra' =>
            '{ type : \'value\' ,
            name : \'  \' ,
            min : 20,
            axisLabel: {
            	formatter : \'{value}  \'
            },
            },',
        'series_option' => ['pipe_leakage_rate' => '
                yAxisIndex:1,
                markLine:{
                    lineStyle:{
                        normal:{
                            type:\'solid\',
                        }
                    },
                    data : [
                        [
                            {
                                cast:\'警戒值:12%\',
                            symbol :\'rect\',
                            symbolSize :1,
                            coord: [0, 12]
                            },
                            {
                                symbol :\'rect\',
                                symbolSize :1,
                                coord: ['.($add-1).', 12]
                            }
                        ],
                    ],
                },',
            ],
    ]
    ];
    $i = 1;
    foreach ($data_settings as $key => $value) {
        @$data_settings[$key]['#option'] = $option[$i - 1];
        $i++;
    }
    return $data_settings;

}

/**
 * 污水再生利用率
 */
function wastewater_reuse_rate($tid)
{

    $data_query = sampling_query($tid,'wastewater_reuse_rate');
    $category = array(
        0 => array(
            'samplingtime' => array('时间'),
            'sewage_treat_capacity' => array('污水处理量', '万m³'),
            'chilledwater' => array('冷却水', '万m³'),
            'irrigationwater' => array('灌溉水', '万m³'),
            'roadcleanwater' => array('道路清洗', '万m³'),
            'ecological_capacity' => array('生态处理水量', '万m³'),
            'reuse_rate' => array('污水再生利用率', '%', 'line'),
        ),
    );
    $chart_name = ['1','2'];

    $option = [
        [
            'yAxis_extra' =>
                '{ type : \'value\' ,
            name : \'  \' ,
            min : 20,
            axisLabel: {
            	formatter : \'{value}  \'
            },
            },',
            'series_option'=>
                [
                    'chilledwater'=>'stack : \'water\',',
                    'irrigationwater'=>'stack : \'water\',',
                    'roadcleanwater'=>'stack : \'water\',',
                    'reuse_rate'=>'yAxisIndex : 1,',
                ]
        ]
    ];
    $data_settings = time_chart2_build($category,$data_query,$chart_name,$option);
    return $data_settings;

}

/**
 * 生态岸线恢复
 */
function ecologicalbankline_data($tid)
{
    $query = db_select('ecologicalbankline_data', 'n');
    $query->fields('n')->condition('t_id', $tid)->orderBy('samplingtime', 'ASC');
    $data = $query->execute();

    $category = array();
    $survival_rate = array();
    $coverage = array();
    $bankline_length = array();
    $species_richness = array();
    $abundance = array();
    foreach ($data as $entry) {
        $samplingtime[] = date('Y年', strtotime($entry->samplingtime));
        $chart_1 [] =
        $survival_rate[] = $entry->survival_rate;
        $coverage[] = $entry->coverage;
        $bankline_length[] = $entry->bankline_length;
        $species_richness[] = $entry->species_richness;
        $abundance[] = $entry->abundance;
    }
    $category = array('成活率', '盖率');
    $query_data[] = $survival_rate;
    $query_data[] = $coverage;

    $legend = $samplingtime;
    $series = build_series($query_data, $legend, build_type(2, 'bar'));
    $xAxis = build_value_axis(array(array(' ', ' ')));
    $yAxis = build_category_axis($category);
    $settings[] = build_settings($series, $legend, $xAxis, $yAxis);

    unset($query_data);
    $category = array('岸线长度', '物种丰富度');
    $query_data[] = $bankline_length;
    $query_data[] = $species_richness;
//    $query_data[] = $abundance;
    $series = build_series($query_data, $legend, build_type(2, 'bar'));
    $xAxis = build_value_axis(array(array(' ', ' ')));
    $yAxis = build_category_axis($category);
    $settings[] = build_settings($series, $legend, $xAxis, $yAxis);
    return $settings;
}


/**
 * 地下水质表格
 */
function groundwater_quality($tid, $date = '')
{
    $output = '';
    //取出第一个日期
    if ($date == '') {
        $date_query = db_select('groundwater_quality', 'n')
            ->fields('n')
            ->condition('t_id', $tid)
            ->orderBy('samplingtime', 'ASE')
            ->range(0, 1)
            ->execute();
        foreach ($date_query as $item) {
            $date = strtotime($item->samplingtime);
            break;
        }
    } else {
        $date = strtotime($date);
    }
    if (empty($date)) {
        $date = time();
    }
    $header = array('监测点', '测定指标', '测定值', '参考值', '是否达标');
    $items = array();
    $date_table = array();

    $query = db_select('groundwater_quality', 'n')
        ->fields('n')
        ->orderBy('equipment', "ASC");

    if (!is_array($tid)) {
        $tid = array($tid);
    }
    foreach ($tid as $item) {
        $query->condition('t_id', $item);
    }

    $result = $query->condition('samplingtime', date('Y-m-d H:i:s', $date))
        ->execute();

    $field = array(
        'PH' => 'PH值',
        'totalhardness' => '总硬度',
        'CODMn' => '高锰酸盐指数',
        'NH3_N' => '氨氮',
        'chloride' => '氯化物',
        'nitrate' => '硝酸盐',
        'fluoride' => '氟化物',
        'FE' => '铁',
        'Ecoil' => '总大肠杆菌'
    );
    $reference = array(
        'PH' => '7',
        'totalhardness' => '8',
        'CODMn' => '12',
        'NH3_N' => '33',
        'chloride' => '44',
        'nitrate' => '32',
        'fluoride' => '12',
        'FE' => '6',
        'Ecoil' => '63'
    );
    foreach ($result as $item) {
        @$result_item = (array)$item;
        foreach ($result_item as $key => $value) {
            if (array_key_exists($key, $field)) {
                $items[] = build_grounderwater_array($result_item['equipment'], $field[$key], $value, $reference[$key], '达标');
            }
        }
    }

    $query_date = db_select('groundwater_quality', 'n')
        ->distinct()
        ->fields('n', array('samplingtime'))
        ->condition('t_id', $tid)
        ->orderBy('samplingtime', 'ASC')
        ->execute();
    foreach ($query_date as $has_date) {
        $format_show = date('Y/m/d', strtotime($has_date->samplingtime));
        $format_time = date('Y-m-d', strtotime($has_date->samplingtime));
        if ($date == strtotime($has_date->samplingtime)) {
            $date_table[] = array(
                l($format_show, 'taxonomy/term/' . $tid[0] . '/report/' . $format_time),
                'active'
            );
        } else {
            $date_table[] = array(
                l($format_show, 'taxonomy/term/' . $tid[0] . '/report/' . $format_time),
                ''
            );
        }
    }
    return array('date' => $date_table, 'header' => $header, 'items' => $items);
}

function build_grounderwater_array($equipment, $field, $value, $reference, $result)
{
    return array($equipment, $field, $value, $reference, $result);
}

/**
 * 水环境质量—地表水
 */
function waterenviroment_quality($tid)
{
    $data_settings = array();
    $date_query = db_select('waterenviroment_quality', 'n')
        ->fields('n')
        ->condition('t_id', $tid)
        ->orderBy('category', 'ASE')
        ->orderBy('samplingtime', 'ASE')
        ->execute();

    //三类参数
    //可重构:后台设置
    $category = array(
        0 => array(
            'samplingtime' => array('时间'),
            'COD' => array('化学需氧量', 'mg/L'),
            'NH3_N' => array('氨氮', 'mg/L'),
            'CODMn' => array('高锰酸盐指数', 'mg/L'),
            'TP' => array('总磷', 'mg/L'),
            'TN' => array('总氮', 'mg/L'),
            'DO' => array('溶解氧', 'mg/L'),
            'SS' => array('悬浮物', 'mg/L'),
        ),
        1 => array(
            'samplingtime' => array('时间'),
            'temperature' => array('温度', '℃'),
            'clarity' => array('透明度', 'cm'),
            'PH' => array('PH'),
            'colority' => array('色度', '度'),
            'TON' => array('臭阈值'),
        ),

    );

    $data_set = array();
    //数据源分类
    foreach ($date_query as $item) {
        $entry = (array)$item;
        $data_set[] = $entry;
        unset($entry['t_id']);
        unset($entry['id']);
        unset($entry['uploadid']);
        $i = 1;
        foreach ($category as $category_1) {
            //添加theme和title
            $chart_name = $item->category . '参数' . $i;
            $chart_drupal = '-' . $chart_name;
            $data_settings[$chart_drupal]['#theme'] = 'tchart_time_chart2';
            $data_settings[$chart_drupal]['#title'] = $chart_name;
            $data_settings[$chart_drupal]['#category'] = $category_1;
            $data_settings[$chart_drupal]['#height'] = 270;
            //添加item
            foreach ($category_1 as $key => $value) {
                $data_settings[$chart_drupal]['#items'][$key][] = $entry[$key];
            }
            $i++;
        }
    }


    //计算水质属性
    $category = array();
    //所有时间
    $timeline = array();
    $origin_items = array();
    foreach ($data_set as $item) {
        $item_time = $item['samplingtime'];
        if (!in_array($item['category'], $category)) {
            $category[] = $item['category'];
        }
        if (!in_array($item['samplingtime'], $timeline)) {
            $timeline[] = $item['samplingtime'];
        }
        $origin_items[$item['category']][$item_time] = $item['quality'];
    }
    if (empty($category)) {
        return array($data_settings);
    }

    //主题,标题
    $items = array('#theme' => 'tchart_time_chart2', '#title' => '水质参数', '#height' => 270,
        '#category' => array(
            'samplingtime' => array('时间'),
        ));
    //添加类别
    foreach ($category as $_category) {
        $items['#category'][$_category] = array(
            $_category . '-水质'
        );
    }
    $ordered_items = array();
    $ordered_items['samplingtime'] = $timeline;
    //水质参数添加到时间轴,缺少的为0
    foreach ($timeline as $_timeline) {
        foreach ($category as $_category) {
            if (isset($origin_items[$_category][$_timeline])) {
                $ordered_items[$_category][] = $origin_items[$_category][$_timeline];
            } else {
                $ordered_items[$_category][] = 0;
            }
        }
    }
    $items['#items'] = $ordered_items;
    return array($data_settings, $items);


}

/**
 * 年径流总量控制率
 */
function streamflow_control_rate($term)
{
    $tid = $term->tid;
    $data_settings = array();
    $date_query = db_select('streamflow_control_rate', 'n')
        ->fields('n')
        ->condition('t_id', $tid)
        ->orderBy('samplingtime', 'ASE')
        ->execute();

    //三类参数
    $category = array(
        0 => array(
            'samplingtime' => array('时间'),
            'flow' => array('流量', 'm^3/s', 'line'),
            'rainfall' => array('降雨量', 'mm', 'line'),
        ),
        1 => array(
            'samplingtime' => array('时间'),
            'streamflow_control_rate' => array('径流控制率', '%'),
        ),
        2 => array(
            'samplingtime' => array('时间'),
            'influentflow' => array('进水量', 'm³'),
            'effluentflow' => array('出水量', 'm³'),
            'reduction_rate' => array('削减率', '%'),
        ),
    );
    $chart_title = ['', 'chart1', 'chart2', 'chart3'];


    $data_set = array();
    //数据源分类
    foreach ($date_query as $item) {
        $entry = (array)$item;
        $data_set[] = $entry;
        unset($entry['t_id']);
        unset($entry['id']);
        unset($entry['uploadid']);
        $i = 1;
        foreach ($category as $category_1) {
            //添加theme和title
            $chart_name = $chart_title[$i];
            $chart_drupal = '-' . $chart_name;
            $data_settings[$chart_drupal]['#theme'] = 'tchart_time_chart2';
//            $data_settings[$chart_drupal]['#title'] = $chart_name;
            $data_settings[$chart_drupal]['#category'] = $category_1;
            $data_settings[$chart_drupal]['#height'] = 270;
            //添加item
            foreach ($category_1 as $key => $value) {
                $data_settings[$chart_drupal]['#items'][$key][] = $entry[$key];
            }
            $i++;
        }
    }
    $add = count(current($data_settings)['#items']['samplingtime']);
    $option = [[
        'yAxis_extra' =>
            '{ type : \'value\' ,
            name : \'  \' ,
            min : 15,
            axisLabel: {
            	formatter : \'{value}  \'
            },
            inverse:true,
            },',
        'series_option' => ['rainfall' => '
                yAxisIndex:1,
                areaStyle: {
                    normal: {}
                },
                markLine:{
                lineStyle:{
                    normal:{
                        type:\'solid\',
                        }
                },
                data : [
    [
        {
            cast:\'平均值:20\',
            symbol :\'rect\',
            symbolSize :1,
            coord: [0, 20]
        },
        {
            symbol :\'rect\',
            symbolSize :1,
            coord: ['.($add-1).', 20]
        }
    ],
                ]
                },',
            'flow' => '
                areaStyle: {
                    normal: {}
                },',],
        'xAxis_option' => 'boundaryGap : false,',
    ]
    ];
    $i = 1;
    foreach ($data_settings as $key => $value) {
        @$data_settings[$key]['#option'] = $option[$i - 1];
        $i++;
    }
    return $data_settings;
}

function build_settings($series, $legend, $xAxis, $yAxis, $containerId = -1)
{

    $settings = array(
        'chartType' => 'line',
        'useType' => array('line', 'bar'),
//        'containerId' =>  'line_chart',
        'options' => array(
            'tooltip' => array('trigger' => 'axis'),
            'legend' => array(
                'data' => $legend,
                'x' => 'center',
            ),
            'toolbox' => array(
                'show' => FALSE,
//                'feature' => array(
//                    'mark' => array('show' => TRUE),
//                    'dataView' => array('show' => TRUE, 'readOnly' => FALSE),
//                    'magicType' => array(
//                        'show' => TRUE,
//                        'type' => array('line', 'bar')
//                    ),
//                    'restore' => array('show' => TRUE),
//                    'saveAsImage' => array('show' => TRUE),
//                )
            ),
            'calculable' => TRUE,
            'xAxis' => $xAxis,
            'yAxis' => $yAxis,
            'series' => $series,
        ),

    );
    if ($containerId != -1) {
        $settings += array('containerId' => $containerId);
    }

    return $settings;
}

function build_time_settings($series, $legend, $xAxis, $yAxis, $containerId = -1)
{

    $settings = array(
        'chartType' => 'line',
        'useType' => array('line', 'bar'),
//        'containerId' =>  'line_chart',
        'options' => array(
            'tooltip' => array('trigger' => 'item', 'formatter' => '{a0}{b0} {b2}'),
            'legend' => array(
                'data' => $legend,
            ),
            'toolbox' => array(
                'show' => TRUE,
                'feature' => array(
                    'mark' => array('show' => TRUE),
                    'dataView' => array('show' => TRUE, 'readOnly' => FALSE),
                    'magicType' => array(
                        'show' => TRUE,
                        'type' => array('line', 'bar')
                    ),
                    'restore' => array('show' => TRUE),
                    'saveAsImage' => array('show' => TRUE),
                )
            ),
            'calculable' => TRUE,
            'xAxis' => $xAxis,
            'yAxis' => $yAxis,
            'series' => $series,
        ),

    );
    if ($containerId != -1) {
        $settings += array('containerId' => $containerId);
    }

    return $settings;
}

function build_series($data, $name, $type, $yAxisIndex = 0)
{


    $series = array();
    $count = 0;
    foreach ($data as $item) {
        $entry = array(
            'name' => $name[$count],
            'type' => $type[$count],
            'data' => $item,
        );
        if (is_array($yAxisIndex)) {
            $entry += array('yAxisIndex' => $yAxisIndex[$count]);
        } else {
            $entry += array('yAxisIndex' => $yAxisIndex);
        }
        $series[] = $entry;
        $count++;
    }
    return $series;
}

function build_time_series($data, $type, $yAxisIndex = 0)
{


    $series = array();
    $count = 0;
//    foreach ($data as $item) {
    $entry = array(
//          'name' => $item['name'],
        'name' => 'serise',
        'type' => $type[$count],
        'showAllSymbol' => TRUE,
        'data' => '(function () {var d = [];var len = 0;var now = new Date();var value;while (len++ < 20) {d.push([new Date(2014, 9, 1, 0, len * 10000),(Math.random()*30).toFixed(2) - 0,(Math.random()*100).toFixed(2) - 0]);}return d;})()',
    );
    if (is_array($yAxisIndex)) {
        $entry += array('yAxisIndex' => $yAxisIndex[$count]);
    } else {
        $entry += array('yAxisIndex' => $yAxisIndex);
    }
    $series[] = $entry;
    $count++;
//    }
    return $series;
}

function build_type($num, $type)
{
    $type_arr = array();
    for ($i = 0; $i < $num; $i++) {
        $type_arr[] = $type;
    }
    return $type_arr;
}

function build_category_axis($category)
{
    return array(
        '0' => array(
            'type' => 'category',
            'data' => $category,
        )
    );
}

function build_time_line_axis($splitNumber = 10)
{
    return array(
        '0' => array(
            'type' => 'time',
            'splitNumber' => $splitNumber,
        )
    );
}

function build_value_axis($name_label)
{
    $axis = array();
    foreach ($name_label as $item) {
        $axis[] = array(
            'type' => 'value',
            'name' => $item[0],
            'axisLabel' => array('formatter' => '{value}' . $item[1]),
            'splitArea' => TRUE
        );
    }
    return $axis;
}

/**
 * 创建时间轴的可渲染数组
 */
function tchart_build_timechart_array($category, $settings, $title = ' ', $unit = array(
    'unit' => '',
    'per' => ''
))
{
    $renderable = array();
    if (!array_key_exists('unit', $unit)) {
        $unit['unit'] = '';
    }
    if (!array_key_exists('per', $unit)) {
        $unit['per'] = '';
    }
    $renderable['#theme'] = 'tchart_time_chart';
    $renderable['#unit'] = $unit;
    $renderable['#items'] = $settings;
    $renderable['#legend'] = $category;
    $renderable['#title'] = $title;
    $renderable['#div_id'] = '';
    return $renderable;
}

function _get_children_tids($tid)
{

    $tids = array($tid);
    if (variable_get('tchart_show_depth_' . $tid, FALSE)) {
        $terms = taxonomy_get_children($tid);
        foreach ($terms as $term) {
            $tids = array_merge($tids, _get_children_tids($term->tid));
        }
    }
    return $tids;
}

function time_chart2_build($category,$date_query,$chart_name,$option=array()){
    $data_settings = array();
    $data_set = array();
    //数据源分类
    foreach ($date_query as $item) {
        $entry = (array)$item;
        $data_set[] = $entry;
        unset($entry['t_id']);
        unset($entry['id']);
        unset($entry['uploadid']);
        $i = 1;
        foreach ($category as $category_1) {
            //添加theme和title
            $chart_drupal = '-' . $chart_name[$i];
            $data_settings[$chart_drupal]['#theme'] = 'tchart_time_chart2';
//            $data_settings[$chart_drupal]['#title'] = $chart_name;
            $data_settings[$chart_drupal]['#category'] = $category_1;
            $data_settings[$chart_drupal]['#height'] = 270;
            //添加item
            foreach ($category_1 as $key => $value) {
                $data_settings[$chart_drupal]['#items'][$key][] = $entry[$key];
            }
            $i++;
        }
    }
    if(!empty($option)) {
        $i = 0;
        foreach ($data_settings as $key => $value) {
            @$data_settings[$key]['#option'] = $option[$i];
            $i++;
        }
    }
    return $data_settings;
}

function sampling_query($tid,$db){
    $data_query = db_select($db, 'n')
        ->fields('n')
        ->condition('t_id', $tid)
        ->orderBy('samplingtime', 'ASE')
        ->execute();
    return $data_query;

}