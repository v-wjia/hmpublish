<?php
/**
 * Created by PhpStorm.
 * User: RachelWaffle
 * Date: 16/2/16
 * Time: 下午12:20
 */


function get_charts($term) {

  $category = variable_get('tchart_term_db_' . $term->tid, 'rainwater_utilization');
  $settings = array();
  $tid = $term->tid;
  $output = '';
  switch ($category) {
	case 'rainwater_utilization':
	  $settings += rainwater_utilization($tid);
	  break;
	case 'pipe_leakage_rate':
	  $settings += pipe_leakage_rate($tid);
	  break;
//        case 'waterenviroment_quality':
//            break;
	case 'wastewater_reuse_rate':
	  $settings += wastewater_reuse_rate($tid);
	  break;
	case 'npsp_data':
	  $settings += npsp_data($tid);
	  break;
	case 'ecologicalbankline_data':
	  $settings += ecologicalbankline_data($tid);
	  break;
	case 'groundwater_quality':
//            $output .= groundwater_quality($tid);
	  break;
	case 'groundwaterlevel':
	  $output .= groundwaterlevel($tid);
	  break;
  }
  if (!empty($settings)) {
	$output = render_echarts(array('echart' => $settings)) . $output;
  }
  return $output;
}

/**
 * 雨水资源利用率
 */
function rainwater_utilization($tid) {

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
function npsp_data($tid) {
  $query = db_select('NPSP_data', 'n');
  $query->fields('n')->condition('t_id', $tid)->orderBy('samplingtime', 'ASC');
  $data = $query->execute();

  $category = array();
  $ph = array();
  $do = array();
  $ss = array();
  $cod = array();
  $nh3_n = array();
  $tn = array();
  $tp = array();
  foreach ($data as $entry) {
	$category[] = date("Y-m-d", strtotime($entry->samplingtime));
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

  $query_data_po[] = $ss;
  $query_data_po[] = $cod;
  $query_data_po[] = $nh3_n;
  $query_data_po[] = $tn;
  $query_data_po[] = $tp;
  $legend_po = array('SS浓度', 'COD浓度', '氨氮浓度', 'TN浓度', 'TP浓度');
  $series_po = build_series($query_data_po, $legend_po, build_type(5, 'bar'));
  $xAxis = build_category_axis($category);
  $yAxis_po = build_value_axis(array(array('污染物浓度', 'mg/L')));
  $settings[] = build_settings($series_po, $legend_po, $xAxis, $yAxis_po);

  $query_data_ph[] = $ph;
  $legend_ph = array('PH');
  $series_ph = build_series($query_data_ph, $legend_ph, build_type(1, 'bar'));
  $xAxis_ph = build_category_axis($category);
  $yAxis_ph = build_value_axis(array(array('PH值', ' ')));
  $settings[] = build_settings($series_ph, $legend_ph, $xAxis_ph, $yAxis_ph);
  return $settings;

  $settings[] = array(
	'chartType' => 'line',
	'useType' => array('line', 'bar'),
	'containerId' => 'line_chart',
	'options' => array(
	  'tooltip' => array('trigger' => 'axis'),
	  'legend' => array(
		'data' => array('SS浓度', 'COD浓度', '氨氮浓度', 'TN浓度', 'TP浓度')
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
	  'xAxis' => array(
		'0' => array(
		  'type' => 'category',
		  'data' => $category,
		)
	  ),
	  'yAxis' => array(
		'0' => array(
		  'type' => 'value',
		  'name' => '污染浓度',
		  'axisLabel' => array('formatter' => '{value}mg/L'),
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

  $settings[] = array(
	'chartType' => 'line',
	'useType' => array('line', 'bar'),
	'containerId' => 'line_chart',
	'options' => array(
	  'tooltip' => array('trigger' => 'axis'),
	  'legend' => array(
		'data' => array('PH值')
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
	  'xAxis' => array(
		'0' => array(
		  'type' => 'category',
		  'data' => $category,
		)
	  ),
	  'yAxis' => array(
		'0' => array(
		  'type' => 'value',
		  'name' => 'PH值',
		  'axisLabel' => array('formatter' => '{value} '),
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
function groundwaterlevel($tid) {

  if (variable_get('tchart_show_deep_' . $tid, FALSE)) {
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
//  $result['legend'] += $temp_result['legend'];
//  $result['settings'] += $temp_result['settings'];
//    }
  //升序排列legend
  asort($result['legend']);
  $set = tchart_build_timechart_array($result['legend'], $result['settings'], '', array(
	'unit' => '深度',
	'per' => 'm'
  ));

  return theme('tchart_time_chart', $set);
//    return $settings;
}

/**
 * 地下水位读取数据库对象数据成为原始数据
 */
function groundwaterlevel_chart($data, $containerId = -1) {

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
	  $chart_set[$name]['data'] = array();
	}

	foreach ($category as $cat) {

	  if ($entry->location == $cat) {
		array_push($chart_set[$cat]['data'], array(
		  strtotime($entry->samplingtime),
		  $entry->groundwaterlevel
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
function pipe_leakage_rate($tid) {
  $query = db_select('pipe_leakage_rate', 'n');
  $query->fields('n')->condition('t_id', $tid)->orderBy('samplingtime', 'ASC');
  $data = $query->execute();

  $category = array();
  $total_water_supply = array();
  $actual_usage = array();
  $pipe_leakage_rate = array();
  foreach ($data as $entry) {
	$category[] = date("Y-m-d", strtotime($entry->samplingtime));
	$total_water_supply[] = $entry->total_water_supply;
	$actual_usage[] = $entry->actual_usage;
	$pipe_leakage_rate[] = $entry->pipe_leakage_rate;
  }

  $query_data[] = $total_water_supply;
  $query_data[] = $actual_usage;
  $query_data[] = $pipe_leakage_rate;

  $legend = array('总供水量', '实际使用量', '管网控制率');
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
 * 污水再生利用率
 */
function wastewater_reuse_rate($tid) {
  $query = db_select('wastewater_reuse_rate', 'n');
  $query->fields('n')->condition('t_id', $tid)->orderBy('samplingtime', 'ASC');
  $data = $query->execute();

  $category = array();
  $sewage_treat_capacity = array();
  $reuse_capacity = array();
  $ecological_capacity = array();
  $reuse_rate = array();
  foreach ($data as $entry) {
	$category[] = date("Y-m-d", strtotime($entry->samplingtime));
	$sewage_treat_capacity[] = $entry->sewage_treat_capacity;
	$reuse_capacity[] = $entry->reuse_capacity;
	$ecological_capacity[] = $entry->ecological_capacity;
	$reuse_rate[] = $entry->reuse_rate;
  }

  $query_data[] = $sewage_treat_capacity;
  $query_data[] = $reuse_capacity;
  $query_data[] = $ecological_capacity;
  $query_data[] = $reuse_rate;

  $legend = array('污水处理量', '再生回用水量', '生态处理水量', '污水再生利用率');
  $series = build_series($query_data, $legend, array(
	'bar',
	'bar',
	'bar',
	'line'
  ), array(0, 0, 0, 1));
  $xAxis = build_category_axis($category);
  $yAxis = build_value_axis(array(array('水量', '万立方米'), array('百分比', '%')));
  $settings[] = build_settings($series, $legend, $xAxis, $yAxis);
  return $settings;
}

/**
 * 生态岸线恢复
 */
function ecologicalbankline_data($tid) {
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
	$category[] = date("Y-m-d", strtotime($entry->samplingtime));
	$survival_rate[] = $entry->survival_rate;
	$coverage[] = $entry->coverage;
	$bankline_length[] = $entry->bankline_length;
	$species_richness[] = $entry->species_richness;
	$abundance[] = $entry->abundance;
  }

  $query_data[] = $survival_rate;
  $query_data[] = $coverage;
  $query_data[] = $bankline_length;
  $query_data[] = $species_richness;
  $query_data[] = $abundance;

  $legend = array('成活率', '盖度', '生态岸线长度', '物种丰富度', '物种多度');
  $series = build_series($query_data, $legend, build_type(5, 'bar'));
  $xAxis = build_value_axis(array(array(' ', ' ')));
  $yAxis = build_category_axis($category);
  $settings[] = build_settings($series, $legend, $xAxis, $yAxis);
  return $settings;
}


/**
 * 地下水质表格
 */
function groundwater_quality($tid, $date = '') {
  $output = '';
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
  }
  else {
	$date = strtotime($date);
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
	@$result_item = (array) $item;
	foreach ($result_item as $key => $value) {
	  if (array_key_exists($key, $field)) {
		$items[] = build_grounderwater_array($result_item['equipment'], $field[$key], $value, $reference[$key], '达标');
	  }
	}
  }

  $query_date = db_select('groundwater_quality', 'n')
	->distinct()
	->fields('n', array('samplingtime'))
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
	}
	else {
	  $date_table[] = array(
		l($format_show, 'taxonomy/term/' . $tid[0] . '/report/' . $format_time),
		''
	  );
	}
  }
  return array('date' => $date_table, 'header' => $header, 'items' => $items);
}

function build_grounderwater_array($equipment, $field, $value, $reference, $result) {
//    $output='';
//    $output.='<table><tr>';
//    $output.='<td>'.$equipment.'</td>';
//    $output.='<td>'.$field.'</td>';
//    $output.='<td>'.$value.'</td>';
//    $output.='<td>'.$reference.'</td>';
//    $output.='<td>'.$result.'</td>';
//    $output.='</tr></table>';
  return array($equipment, $field, $value, $reference, $result);
}


function build_settings($series, $legend, $xAxis, $yAxis, $containerId = -1) {

  $settings = array(
	'chartType' => 'line',
	'useType' => array('line', 'bar'),
//        'containerId' =>  'line_chart',
	'options' => array(
	  'tooltip' => array('trigger' => 'axis'),
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

function build_time_settings($series, $legend, $xAxis, $yAxis, $containerId = -1) {

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

function build_series($data, $name, $type, $yAxisIndex = 0) {


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
	}
	else {
	  $entry += array('yAxisIndex' => $yAxisIndex);
	}
	$series[] = $entry;
	$count++;
  }
  return $series;
}

function build_time_series($data, $type, $yAxisIndex = 0) {


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
  }
  else {
	$entry += array('yAxisIndex' => $yAxisIndex);
  }
  $series[] = $entry;
  $count++;
//    }
  print_r($series);
  return $series;
}

function build_type($num, $type) {
  $type_arr = array();
  for ($i = 0; $i < $num; $i++) {
	$type_arr[] = $type;
  }
  return $type_arr;
}

function build_category_axis($category) {
  return array(
	'0' => array(
	  'type' => 'category',
	  'data' => $category,
	)
  );
}

function build_time_line_axis($splitNumber = 10) {
  return array(
	'0' => array(
	  'type' => 'time',
	  'splitNumber' => $splitNumber,
	)
  );
}

function build_value_axis($name_label) {
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
function tchart_build_timechart_array($category, $settings, $title = '', $unit = array(
  'unit' => '',
  'per' => ''
)) {
  $renderable = array();
  if (!array_key_exists('unit', $unit)) {
	$unit['unit'] = '';
  }
  if (!array_key_exists('per', $unit)) {
	$unit['per'] = '';
  }
  $renderable['unit'] = $unit;
  $renderable['items'] = $settings;
  $renderable['legend'] = $category;
  $renderable['title'] = $title;
  $renderable['div_id'] = '';
  return $renderable;
}