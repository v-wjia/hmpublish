<?php
/**
 * Created by PhpStorm.
 * User: RachelWaffle
 * Date: 16/2/15
 * Time: 下午5:17
 */

function get_all_term_parents($term)
{
    $parents_str = '';
    $parents = taxonomy_get_parents_all($term->tid);
    if (!empty($parents)) {

        foreach (array_reverse($parents) as $parent) {
            $parents_str .= '/' . $parent->name;
        }
    }
    return $parents_str;
}


/**
 * 根据深度加载可渲染的array
 */
function get_vocabulary_renderable_array($single_term, $depth, $name, $id, $id_name = 'tid')
{
    $tid = $single_term->tid;
    for ($i = 0; $i < $depth; $i++) {
        $single_terms = taxonomy_get_children($tid);
        if (!empty($single_terms)) {
            $single_term = current($single_terms);
            $tid = $single_term->tid;
        } else {
            unset($single_term);
        }
    }
    if (!empty($single_term)) {
        $items = array(
            'data' => array('title' => $name, 'link' => 'taxonomy/term/' . $single_term->tid),
            $id_name => $id,
        );
    } else {
        $items = array(
            'data' => array('title' => $name, 'link' => ''),
            $id_name => $id,
        );
    }
    return $items;
}

/**
 * 根据tid获得对应深度的url
 * 如layer A1的tid是1,其层级关系为
 * layer A1-|
 *          |-layer B1-|
 *          |-layer B2 |-layer C1
 *                     |-layer C2
 * 使用get_term_depth_url(1,2)将返回layer C1的url
 */
function get_term_depth_url($tid, $max_depth = 1)
{
    $depth_tid = $tid;
    for ($i = 0; $i < $max_depth; $i++) {
        $single_terms = taxonomy_get_children($depth_tid);
        if (!empty($single_terms)) {
            $single_term = current($single_terms);
            $depth_tid = $single_term->tid;
        } else {
            break;
        }
    }
    return 'taxonomy/term/' . $depth_tid;
}


function tchart_get_block_content($tid, $depth)
{
    global $tchart_cache_content;


    $data = array_reverse(taxonomy_get_parents_all($tid));
    $vid = taxonomy_term_load($tid)->vid;

    $i = 1;
    $items = array();
    $active = array();
    $terms = taxonomy_get_tree($vid, 0, 1);
    //获取vocabulary下的term
    foreach ($terms as $term) {
        $items[$i][] = array(
            //根据深度调节url
            'data' => array('title' => $term->name, 'link' => get_term_depth_url($term->tid, $depth - $i)), //取出每一个发表的文章
            //标记tid,根据是否对应层级添加class active
            'tid' => $term->tid,
        );
    }
    $i++;

    //获取其余子term
    foreach ($data as $item) {
        $active[] = $item->tid;
    }
    foreach ($data as $item) {

        //到达最大深度则更深term数据不显示
        if ($i > $depth) {
            break;
        }
        $terms = taxonomy_get_tree($vid, $item->tid, 1);
        foreach ($terms as $term) {
            if ((variable_get('tchart_show_depth_' . $term->tid, FALSE) || variable_get('tchart_show_children_' . $term->tid, FALSE)) && in_array($term->tid, $active)) {
                $data_array = _get_children($term, $active);
            } else {
                $data_array = array(
                    //根据深度调节url
                    'data' => array('title' => $term->name, 'link' => get_term_depth_url($term->tid, $depth - $i)), //取出每一个发表的文章
                    'tid' => $term->tid,
                );
            }
            $items[$i][] = $data_array;
        }
        $i++;
    }
    $tchart_cache_content = array('items' => $items, 'active' => $active);

}

function _get_children($term, $active)
{
    $data_array = array(
        //根据深度调节url
        'data' => array('title' => $term->name, 'link' => 'taxonomy/term/' . $term->tid),
        'tid' => $term->tid,
    );
    $children = array();
    if ((variable_get('tchart_show_depth_' . $term->tid, FALSE) || variable_get('tchart_show_children_' . $term->tid, FALSE)) && in_array($term->tid, $active)) {
        $children_terms = taxonomy_get_children($term->tid);
        foreach ($children_terms as $children_term) {
            $children[] = _get_children($children_term, $active);
        }
    }
    if (!empty($children)) {
        $data_array['children'] = $children;
    }
    return $data_array;
}

function tchart_render_navigation($items, $active, $title, $theme = 'tchart_side_nav', $active_id = 'vid')
{
    //渲染
    if (empty($items)) {
        $output = '';
    } else {
        $output = theme($theme, array('items' => $items, 'active' => $active, 'active_id' => $active_id, 'title' => $title));
    }
    return $output;
}

/**
 * 获得当前页面的tid和vid
 */
function get_page_term()
{
    $node = menu_get_object();
    $term = menu_get_object('taxonomy_term', 2);
    if (!empty($node)) {
        //测试:field_name
        $current_term = current(current($node->field_attachcategory))['taxonomy_term'];
        return array('vid' => $current_term->vid, 'tid' => $current_term->tid);
    } elseif (!empty($term)) {
        return array('vid' => $term->vid, 'tid' => $term->tid);
    } elseif (drupal_is_front_page()) {
        return array('vid' => -1, 'tid' => -1);
    }
    return array('vid' => -1, 'tid' => -2);
}

/**
 * 获取taxonomy有图片的node
 */
function tchart_taxonomy_select_nodes($tid, $pager = TRUE, $limit = FALSE, $order = array('t.sticky' => 'DESC', 't.created' => 'DESC'))
{
    if (!variable_get('taxonomy_maintain_index_table', TRUE)) {
        return array();
    }
    $query = db_select('taxonomy_index', 't');
    $query->addTag('node_access');
    $query->condition('tid', $tid);

    $query->join('field_data_field_image', 'no', 'no.entity_id = t.nid');
    $query->condition('entity_type', 'node');

    $query->join('file_managed', 'fm', 'fm.fid=no.field_image_fid');
    $query->addField('fm', 'uri');


    if ($pager) {
        $count_query = clone $query;
        $count_query->addExpression('COUNT(t.nid)');

        $query = $query->extend('PagerDefault');
        if ($limit !== FALSE) {
            $query = $query->limit($limit);
        }
        $query->setCountQuery($count_query);
    } else {
        if ($limit !== FALSE) {
            $query->range(0, $limit);
        }
    }
    $query->addField('t', 'nid');
    $query->addField('t', 'tid');
    foreach ($order as $field => $direction) {
        $query->orderBy($field, $direction);
        // ORDER BY fields need to be loaded too, assume they are in the form
        // table_alias.name
        list($table_alias, $name) = explode('.', $field);
        $query->addField($table_alias, $name);
    }


    return $query->execute();
}

/**
 * 根据报告获得下一级标题
 */
function _get_check($items, $active)
{
    $check = '考核区域';
    foreach ($items as $item) {
        if ($item['data']['title'] == '报告') {
            if (in_array($item['tid'], $active)) {
                $check = '考核报告';
                break;
            }
        }
    }
    return $check;
}

//获得联动的组
function _get_chart_connect(){
    global $chart_connect;
    $output = '';
    if(!empty($chart_connect)) {
        $output.='<script type="text/javascript">';
        foreach ($chart_connect as $group => $chart) {
            $output .= "echarts.connect('$group');";
        }
        $output.='</script>';
    }
    return $output;
}

/**
 * 获得选项
 */
function _chart_option($option)
{
    $std_option = new stdClass();
    //默认不反转xy
    $std_option->reverse = isset($option['reverse']) ? $option['reverse'] : FALSE;
    //默认显示时间区域
    $std_option->show_zoom = isset($option['show_zoom']) ? $option['show_zoom'] : TRUE;
    //赋值series选项
    $std_option->series_option = isset($option['series_option']) ? $option['series_option'] : array();
    //赋值xAxis选项
    $std_option->xAxis_option = isset($option['xAxis_option']) ? $option['xAxis_option'] : "";
    //赋值xAxis选项
    $std_option->xAxis_extra = isset($option['xAxis_extra']) ? $option['xAxis_extra'] : "";
    //赋值yAxis选项
    $std_option->yAxis_option = isset($option['yAxis_option']) ? $option['yAxis_option'] : "";
    //赋值yAxis选项
    $std_option->yAxis_extra = isset($option['yAxis_extra']) ? $option['yAxis_extra'] : "";
    //赋值legend选项
    $std_option->legend_option = isset($option['legend_option']) ? $option['legend_option'] : "";
    //赋值类目轴选项
    $std_option->timespan = isset($option['timespan']) ? $option['timespan'] : TRUE;
    //赋值数值轴单位符号
    $std_option->yAxis_per = isset($option['yAxis_per']) ? $option['yAxis_per'] : " ";
    //赋值数值轴单位名称
    $std_option->yAxis_unit = isset($option['yAxis_unit']) ? $option['yAxis_unit'] : " ";
    //赋值x轴为类目轴
    $std_option->x_is_category = isset($option['x_is_category']) ? $option['x_is_category'] : FALSE;
    //赋值组(多图表联动)
    $std_option->chart_connect = isset($option['chart_connect']) ? $option['chart_connect'] : 'none_connect';

    return $std_option;
}