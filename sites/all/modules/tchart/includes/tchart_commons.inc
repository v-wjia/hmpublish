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

