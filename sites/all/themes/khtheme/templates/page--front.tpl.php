<?php

/**
 * @file
 * khtheme's theme implementation to display a single Drupal page.
 *
 * The doctype, html, head and body tags are not in this template. Instead they
 * can be found in the html.tpl.php template normally located in the
 * modules/system directory.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/khtheme.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 * - $hide_site_name: TRUE if the site name has been toggled off on the theme
 *   settings page. If hidden, the "element-invisible" class is added to make
 *   the site name visually hidden, but still accessible.
 * - $hide_site_slogan: TRUE if the site slogan has been toggled off on the
 *   theme settings page. If hidden, the "element-invisible" class is added to
 *   make the site slogan visually hidden, but still accessible.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['header']: Items for the header region.
 * - $page['featured']: Items for the featured region.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['triptych_first']: Items for the first triptych.
 * - $page['triptych_middle']: Items for the middle triptych.
 * - $page['triptych_last']: Items for the last triptych.
 * - $page['footer_firstcolumn']: Items for the first footer column.
 * - $page['footer_secondcolumn']: Items for the second footer column.
 * - $page['footer_thirdcolumn']: Items for the third footer column.
 * - $page['footer_fourthcolumn']: Items for the fourth footer column.
 * - $page['footer']: Items for the footer region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 * @see khtheme_process_page()
 * @see html.tpl.php
 */
?>
<div id="page-wrapper">
  <div id="page">


    <div class="header">
        <?php if ($site_name): ?>

             <h3><?php print $site_name; ?></h3>
        <?php endif; ?>
      <?php if ($page['highlighted']): ?><div id="highlighted"><div class="login"><?php print render($page['highlighted']); ?></div></div><?php endif; ?>


  </div>
    <!-- /.section, /#header -->

  <?php if ($messages): ?>
    <div id="messages"><div class="section clearfix">
      <?php print $messages; ?>
    </div></div> <!-- /.section, /#messages -->
  <?php endif; ?>



<div class="main">


    <div class="left">
      <div class="leftimg">
        <span><?php echo  date('Y年m月d日 ',time()).getWeek();?></span>
      </div>

    <?php if ($page['sidebar_first']): ?>
      <div id="sidebar-first" class="column sidebar">
          <div class="section">
            <?php print render($page['sidebar_first']); ?>
          </div>
      </div> <!-- /.section, /#sidebar-first -->
    <?php endif; ?>
  </div>

<div class="right">

  <?php if ($page['featured']): ?>
    <div id="featured">
        <?php print render($page['featured']); ?>
    </div> <!-- /.section, /#featured -->
  <?php endif; ?>


<!--    <div id="content" class="column"><div class="section">-->
<!---->
<!--      <a id="main-content"></a>-->
<!--      --><?php //print render($title_prefix); ?>
<!--      --><?php //if ($title): ?>
<!--        <h1 class="title" id="page-title">-->
<!--          --><?php //print $title; ?>
<!--        </h1>-->
<!--      --><?php //endif; ?>
<!--      --><?php //print render($title_suffix); ?>
<!--      --><?php //if ($tabs): ?>
<!--        <div class="tabs">-->
<!--          --><?php //print render($tabs); ?>
<!--        </div>-->
<!--      --><?php //endif; ?>
<!--      --><?php //print render($page['help']); ?>
<!--      --><?php //if ($action_links): ?>
<!--        <ul class="action-links">-->
<!--          --><?php //print render($action_links); ?>
<!--        </ul>-->
<!--      --><?php //endif; ?>
<!--      --><?php //print render($page['content']); ?>
<!--      --><?php //print $feed_icons; ?>
<!---->
<!--    </div></div>-->
  <script type="text/javascript">
    jQuery(function(){

    });
  </script>



<!--  <div class="sk-wrapper">
    <div class="sk sk1">
      <div class="sk-logo"><span>规划区域</span></div>
      <p><span>区域面积</span>&nbsp;&nbsp;<span>160平方公里</span></p>
      <p><span>区域数量</span>&nbsp;&nbsp;<span><em>6</em>个</span></p>
    </div>
    <div class="sk sk2">
      <div class="sk-logo"><span>监测指标</span></div>
      <p><span>定量指标</span>&nbsp;&nbsp;<span>11个</span></p>
      <p><span>定性指标</span>&nbsp;&nbsp;<span>7个</span></p>
      <p><span>在线监控</span>&nbsp;&nbsp;<span><em>5</em>个</span></p>
    </div>
    <div class="sk sk3">
      <div class="sk-logo"><span>硬件设备</span></div>
      <p><span>传感器数量</span>&nbsp;&nbsp;<span>11个</span></p>
      <p><span>站点数量</span>&nbsp;&nbsp;&nbsp;&nbsp;<span>7个</span></p>
      <p><span>覆盖面积</span>&nbsp;&nbsp;&nbsp;&nbsp;<span><em>5</em>个</span></p>
    </div>
  </div>


  <div class="zb-wrapper">
    <div class="zb-left">
      <h5>定量指标<img src="<?php echo $site_path; ?>img/ddd.jpg" alt=""></h5>
      <ul>
        <li>年径流总量控制率<div class="chart red"><div style="width:70px"></div></li>
        <li>生态岸线恢复<div class="chart green"><div style="width:50px"></div></div></li>
        <li>地下水位<div class="chart red"><div style="width:70px"></div></div></li>
        <li>城市热岛效应<div class="chart green"><div style="width:50px"></div></div></li>
        <li>水环境质量<div class="chart green"><div style="width:20px"></div></div></li>
        <li>城市面源污染控制<div class="chart red"><div style="width:100px"></div></div></li>
        <li>污水再生利用率<div class="chart green"><div style="width:30px"></div></div></li>
        <li>雨水资源利用率<div class="chart red"><div style="width:20px"></div></div></li>
        <li>管网漏损控制<div class="chart red"><div style="width:60px"></div></div></li>
        <li>城市暴雨内涝灾害防治<div class="chart green"><div style="width:30px"></div></div></li>
        <li>饮用水安全<div class="chart red"><div style="width:80px"></div></div></li>
      </ul>
    </div>
    <div class="zb-right">
      <h5>定性指标<img src="<?php echo $site_path; ?>img/ddd.jpg" alt=""></h5>
      <ul>
        <li>规划建设管控制度<img src="<?php echo $site_path; ?>img/dg.png"></li>
        <li>蓝线、绿线划定与保护<img src="<?php echo $site_path; ?>img/dg.png"></li>
        <li>技术规范与标准建设<img src="<?php echo $site_path; ?>img/dg.png"></li>
        <li>投融次机制建设<img src="<?php echo $site_path; ?>img/dg.png"></li>
        <li>绩效考核与奖励机制<img src="<?php echo $site_path; ?>img/dg.png"></li>
        <li>产业化<img src="<?php echo $site_path; ?>img/dg.png"></li>
        <li style="border-bottom:0">连片示范效应<img src="/<?php echo drupal_get_path('theme','khtheme');?>/img/dg.png"></li>
      </ul>
      <img src="<?php echo $site_path; ?>img/chart.png" alt="" style="display:block;margin:30px auto 43px;">
    </div>
  </div>-->

    <div class="sk-wrapper">
        <div class="new_sk new_sk1">
            <div class="sk-logo"><span>规划区域</span></div>
            <p><span>区域面积</span>&nbsp;&nbsp;<span>160平方公里</span></p>
            <p><span>区域数量</span>&nbsp;&nbsp;<span><em>6</em>个</span></p>
        </div>
        <div class="new_sk new_sk2">
            <div class="sk-logo"><span>监测指标</span></div>
            <p><span>定量指标</span>&nbsp;&nbsp;<span>11个</span></p>
            <p><span>定性指标</span>&nbsp;&nbsp;<span>7个</span></p>
            <p><span>在线监控</span>&nbsp;&nbsp;<span><em>5</em>个</span></p>
        </div>
        <div class="new_sk new_sk3">
            <div class="sk-logo"><span>硬件设备</span></div>
            <p><span>传感器数量</span>&nbsp;&nbsp;<span>11个</span></p>
            <p><span>站点数量</span>&nbsp;&nbsp;&nbsp;&nbsp;<span>7个</span></p>
            <p><span>覆盖面积</span>&nbsp;&nbsp;&nbsp;&nbsp;<span><em>5</em>个</span></p>
        </div>
    </div>


    <div class="zb-wrapper">
        <div class="zb-col-con">
            <div class="zb-col up">
                <h5>水生态<img src="<?php echo $site_path; ?>img/ddd.jpg" alt=""></h5>
                <ul>
                    <li>年径流总量控制率<span class="val">87.3%</span></li>
                    <li>生态岸线恢复<span class="val">10km</span></li>
                    <li>地下水位<span class="val">5.48m</span></li>
                    <li>城市热岛效应<span class="val">20</span></li>
                </ul>
            </div>
            <div class="zb-col down">
                <h5>水环境<img src="<?php echo $site_path; ?>img/ddd.jpg" alt=""></h5>
                <ul>
                    <li>水环境质量<span class="val">IV类</span></li>
                    <li>城市面源污染控制<span class="val">IV类</span></li>
                </ul>
            </div>
        </div>
        <div class="zb-col-con">
            <div class="zb-col up" style="margin-right:0">
                <h5>水资源<img src="<?php echo $site_path; ?>img/ddd.jpg" alt=""></h5>
                <ul>
                    <li>污水再生利用率<span class="val">22%</span></li>
                    <li>雨水资源利用率<span class="val">8.50%</span></li>
                    <li>管网漏损控制<span class="val">11%</span></li>
                </ul>
            </div>
            <div class="zb-col down">
                <h5>水安全<img src="<?php echo $site_path; ?>img/ddd.jpg" alt=""></h5>
                <ul>
                    <li>城市暴雨内涝灾害防治<span class="val"></span></li>
                    <li>饮用水安全<span class="val">Ⅲ类</span></li>
                </ul>
            </div>
        </div>
        <div class="zb-col-con" style="margin-right:0">
            <div class="zb-col up">
                <h5>制度建设及执行情况<img src="<?php echo $site_path; ?>img/ddd.jpg" alt=""></h5>
                <ul>
                    <li>规划建设管控制度<span class="val">已规划</span></li>
                    <li>蓝线、绿线划定与保护<span class="val">已规划</span></li>
                    <li>技术规范与标准建设<span class="val">已规划</span></li>
                    <li>投融资机制建设<span class="val">已规划</span></li>
                    <li>绩效考核与奖励机制<span class="val">已规划</span></li>
                    <li>产业化<span class="val">已规划</span></li>
                </ul>
            </div>
            <div class="zb-col down">
                <h5>显示度<img src="<?php echo $site_path; ?>img/ddd.jpg" alt=""></h5>
                <ul>
                    <li>连片示范效应<span class="val">62%</span></li>
                </ul>
            </div>
        </div>
    </div>
  <!-- /.section, /#content -->

    <?php if ($page['sidebar_second']): ?>
      <div id="sidebar-second" class="column sidebar"><div class="section">
        <?php print render($page['sidebar_second']); ?>
      </div></div> <!-- /.section, /#sidebar-second -->
    <?php endif; ?>

  </div>
<!--  /.right-->

  </div>
<!--  </div> -->
  <!-- /#main, /#main-wrapper -->

  <?php if ($page['triptych_first'] || $page['triptych_middle'] || $page['triptych_last']): ?>
    <div id="triptych-wrapper"><div id="triptych" class="clearfix">
      <?php print render($page['triptych_first']); ?>
      <?php print render($page['triptych_middle']); ?>
      <?php print render($page['triptych_last']); ?>
    </div></div> <!-- /#triptych, /#triptych-wrapper -->
  <?php endif; ?>

<!--  <div id="footer-wrapper">-->
<!--    <div class="section">-->

    <?php if ($page['footer_firstcolumn'] || $page['footer_secondcolumn'] || $page['footer_thirdcolumn'] || $page['footer_fourthcolumn']): ?>
      <div id="footer-columns" class="clearfix">
        <?php print render($page['footer_firstcolumn']); ?>
        <?php print render($page['footer_secondcolumn']); ?>
        <?php print render($page['footer_thirdcolumn']); ?>
        <?php print render($page['footer_fourthcolumn']); ?>
      </div> <!-- /#footer-columns -->
    <?php endif; ?>

    <?php if ($page['footer']): ?>
      <div id="footer" class="clearfix">
        <?php print render($page['footer']); ?>
      </div> <!-- /#footer -->
    <?php endif; ?>

<!--  </div>-->
<!--  </div>-->
  <!-- /.section, /#footer-wrapper -->

</div>
</div>
<!-- /#page, /#page-wrapper -->
