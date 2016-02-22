<?php
/**
 * Created by PhpStorm.
 * User: RachelWaffle
 * Date: 16/2/15
 * Time: 下午5:17
 */

function get_all_term_parents($term){
    $parents_str = '';
    $parents = taxonomy_get_parents_all($term->tid);
    if(!empty($parents)){

        foreach(array_reverse($parents) as $parent){
            $parents_str .='/'.$parent->name;
        }
    }
    return $parents_str;
}


/**
 * 根据深度加载可渲染的array
 */
function get_vocabulary_renderable_array($single_term, $depth, $name, $id, $id_name='tid'){
    $tid=$single_term->tid;
    for($i=0;$i<$depth;$i++){
        $single_terms= taxonomy_get_children($tid);
        if(!empty($single_terms)){
            $single_term=current($single_terms);
            $tid=$single_term->tid;
        }else{
            unset($single_term);
        }
    }
    if (!empty($single_term)) {
        $items = array(
            'data' => l($name, 'taxonomy/term/' . $single_term->tid),
            $id_name => $id,
        );
    } else {
        $items = array(
            'data' => $name,
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
function get_term_depth_url($tid,$max_depth=1){
    $depth_tid=$tid;
    for($i=0;$i<$max_depth;$i++){
        $single_terms= taxonomy_get_children($depth_tid);
        if(!empty($single_terms)) {
            $single_term = current($single_terms);
            $depth_tid=$single_term->tid;
        }else{
            break;
        }
    }
    return 'taxonomy/term/'.$depth_tid;
}


function tchart_get_block_content($tid,$depth){
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
            'data' => l($term->name, get_term_depth_url($term->tid,$depth-$i)), //取出每一个发表的文章
            //标记tid,根据是否对应层级添加class active
            'tid' => $term->tid,
        );
    }
    $i++;

    //获取其余子term
    foreach ($data as $item) {
        $active[] = $item->tid;
        //到达最大深度则更深term数据不显示
        if($i>$depth){
            break;
        }
        $terms = taxonomy_get_tree($vid, $item->tid, 1);
        foreach ($terms as $term) {
            $items[$i][] = array(
                //根据深度调节url
                'data' => l($term->name, get_term_depth_url($term->tid,$depth-$i)), //取出每一个发表的文章
                'tid' => $term->tid,
            );
        }
        $i++;
    }
    $tchart_cache_content=array('items'=>$items,'active'=>$active);
}

function tchart_render_navigation($items, $active, $theme='tchart_side_nav', $active_id='vid'){
    //渲染
    if (empty($items)) {
        $output = '';
    } else {
        $output = theme($theme, array('items' => $items, 'active' => $active,'active_id'=>$active_id)) . '<br>';
    }
    return $output;
}

/**
 * 获得当前页面的tid和vid
 */
function get_page_term(){
    $node=menu_get_object();
    $term=menu_get_object('taxonomy_term',2);
    if(!empty($node)){
        //测试:field_name
        $current_term=current(current($node->field_22222))['taxonomy_term'];
        return array('vid'=>$current_term->vid,'tid'=>$current_term->tid);
    }elseif(!empty($term)){
        return array('vid'=>$term->vid,'tid'=>$term->tid);
    }elseif(drupal_is_front_page()){
        return array('vid'=>-1,'tid'=>-1);
    }
    return array('vid'=>-1,'tid'=>-2);
}
