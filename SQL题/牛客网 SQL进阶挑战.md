# 牛客网 SQL进阶挑战

## SQL135  每个6/7级用户活跃情况
```sql
SELECT
  uid,
  COUNT(DISTINCT start_month) AS act_month_total,
  COUNT(DISTINCT (IF(start_year = '2021', start_date, NULL))) AS act_days_2021,
  COUNT(
    DISTINCT (
      IF(
        start_year = '2021'
        AND u_type = 'exam',
        start_date,
        NULL
      )
    )
  ) AS act_days_2021_exam,
  COUNT(
    DISTINCT (
      IF(
        start_year = '2021'
        AND u_type = 'practice',
        start_date,
        NULL
      )
    )
  ) AS act_days_2021_question
FROM
  (
    SELECT
      ui.uid,
      er.start_time,
      YEAR(er.start_time) AS start_year,
      DATE_FORMAT(er.start_time, '%Y%m') AS start_month,
      DATE_FORMAT(er.start_time, '%Y%m%d') AS start_date,
      'exam' AS u_type
    FROM
      exam_record er
      RIGHT JOIN user_info ui ON er.uid = ui.uid
    WHERE
      ui.level IN (6, 7)
    UNION ALL
    SELECT
      ui.uid,
      pr.submit_time AS start_time,
      YEAR(pr.submit_time) AS start_year,
      DATE_FORMAT(pr.submit_time, '%Y%m') AS start_month,
      DATE_FORMAT(pr.submit_time, '%Y%m%d') AS start_date,
      'practice' AS u_type
    FROM
      practice_record pr
      RIGHT JOIN user_info ui ON pr.uid = ui.uid
    WHERE
      ui.level IN (6, 7)
  ) p
GROUP BY
  uid
ORDER BY
  act_month_total DESC,
  act_days_2021 DESC;
```

## SQL136 每类试卷得分前3名

```sql
SELECT  tag AS tid
       ,uid
       ,ranking
FROM (
SELECT  i.tag
       ,uid
       ,ROW_NUMBER() OVER(PARTITION BY i.tag ORDER BY MAX(r.score) DESC,MIN(r.score) DESC,uid DESC) AS ranking
FROM examination_info i
INNER JOIN exam_record r
ON i.exam_id = r.exam_id
WHERE r.submit_time IS NOT NULL
AND r.score IS NOT NULL
GROUP BY  i.tag
         ,uid )p
WHERE ranking <= 3;
```
##### Note
要学习掌握窗口函数在group by 之后执行的用法，可以看做是对group by的结果做开窗，聚合函数也可以当作一列使用在窗口函数中

## SQL137 第二快/慢用时之差大于试卷时长一半的试卷

```sql
SELECT  p1.exam_id
       ,p1.duration
       ,p1.release_time
FROM
(
	SELECT  exam_id
	       ,duration
	       ,release_time
	       ,time_gap
	FROM
	(
		SELECT  i.exam_id
		       ,i.duration
		       ,i.release_time
		       ,TIMESTAMPDIFF(SECOND,start_time,submit_time)/60                                                        AS time_gap
		       ,ROW_NUMBER() OVER(PARTITION BY i.exam_id ORDER BY TIMESTAMPDIFF(SECOND,start_time,submit_time)/60  DESC) AS ranking
		FROM examination_info i
		INNER JOIN exam_record r
		ON i.exam_id = r.exam_id
		WHERE r.submit_time IS NOT NULL 
	) s
	WHERE ranking = 2 
) p1
LEFT JOIN
(
	SELECT  exam_id
	       ,duration
	       ,release_time
	       ,time_gap
	FROM
	(
		SELECT  i.exam_id
		       ,i.duration
		       ,i.release_time
		       ,TIMESTAMPDIFF(SECOND,start_time,submit_time)/60                                                   AS time_gap
		       ,ROW_NUMBER() OVER(PARTITION BY i.exam_id ORDER BY TIMESTAMPDIFF(SECOND,start_time,submit_time)/60 ) AS ranking
		FROM examination_info i
		INNER JOIN exam_record r
		ON i.exam_id = r.exam_id
		WHERE r.submit_time IS NOT NULL 
	) s
	WHERE ranking = 2 
) p2
ON p1.exam_id = p2.exam_id
WHERE (p1.time_gap-p2.time_gap) > p1.duration/2
ORDER BY p1.exam_id DESC;
```

## SQL138 连续两次作答试卷的最大时间窗

```sql
SELECT
  uid,
  days_window,
  ROUND((amt / (DATEDIFF(max, min) + 1)) * days_window, 2) AS avg_exam_cnt
FROM
  (
    SELECT
      uid,
      COUNT(exam_id) AS amt,
      MAX(start_time) AS max,
      MIN(start_time) AS min,
      MAX(DATEDIFF(big, start_time) + 1) AS days_window
    FROM
      (
        SELECT
          uid,
          exam_id,
          start_time,
          lead(start_time) OVER(
            PARTITION BY uid
            ORDER BY
              start_time
          ) AS big
        FROM
          exam_record
        WHERE
          year(start_time) = '2021'
      ) s
    GROUP BY
      uid
  ) AS p
WHERE
  DATEDIFF(max, min) > 0
ORDER BY
  days_window DESC,
  avg_exam_cnt DESC;
```

## SQL139 近三个月未完成试卷数为0的用户完成情况

```sql
SELECT  uid
       ,SUM(exam_cnt) AS exam_complete_cnt
FROM
(
	SELECT  uid
	       ,YEAR(start_time)                                                                          AS year
	       ,MONTH(start_time)                                                                         AS month
	       ,COUNT(*)                                                                                  AS exam_cnt
	       ,SUM(IF(submit_time IS NULL,1,0))                                                          AS null_cnt
	       ,ROW_NUMBER() OVER(PARTITION BY uid ORDER BY YEAR(start_time) DESC,MONTH(start_time) DESC) AS ranking
	FROM exam_record r
	GROUP BY  uid
	         ,YEAR(start_time)
	         ,MONTH(start_time)
) s
WHERE ranking <= 3
GROUP BY  uid
HAVING SUM(null_cnt) = 0
ORDER BY exam_complete_cnt DESC, uid DESC;
```

## SQL140 未完成率较高的50%用户近三个月答卷情况
```sql
WITH tmp AS (
  SELECT
    er.*,
    ei.tag,
    ui.level
  FROM
    exam_record er
    LEFT JOIN examination_info ei ON er.exam_id = ei.exam_id
    LEFT JOIN user_info ui ON er.uid = ui.uid
)
SELECT
  s1.uid,
  s2.start_month,
  total_cnt,
  complete_cnt
FROM
  (
    SELECT
      uid,
      MAX(level) AS level,
      COUNT(submit_time) / COUNT(1) AS rate,
      ntile(2) OVER(
        ORDER BY
          (1 - COUNT(submit_time) / COUNT(1)) DESC
      ) AS ranking
    FROM
      tmp
    WHERE
      tag = 'SQL'
    GROUP BY
      uid
  ) s1
  JOIN (
    SELECT
      uid,
      DATE_FORMAT(start_time, '%Y%m') AS start_month,
      COUNT(1) AS total_cnt,
      COUNT(submit_time) AS complete_cnt,
      ROW_NUMBER() OVER(
        PARTITION BY uid
        ORDER BY
          DATE_FORMAT(start_time, '%Y%m') DESC
      ) AS ranking
    FROM
      tmp
    GROUP BY
      uid,
      DATE_FORMAT(start_time, '%Y%m')
  ) s2 ON s1.uid = s2.uid
WHERE
  s1.ranking = 1
  AND s1.level IN (6, 7)
  AND s2.ranking <= 3
ORDER BY
  uid,
  start_month;
```

```sql
-- 方法二，使用另一种窗口函数
WITH tmp AS (
  SELECT
    er.*,
    ei.tag,
    ui.level
  FROM
    exam_record er
    LEFT JOIN examination_info ei ON er.exam_id = ei.exam_id
    LEFT JOIN user_info ui ON er.uid = ui.uid
)
SELECT
  s1.uid,
  s2.start_month,
  total_cnt,
  complete_cnt
FROM
  (
    SELECT
      uid,
      MAX(level) AS level,
      COUNT(submit_time) / COUNT(1) AS rate,
      PERCENT_RANK() OVER(
        ORDER BY
          (1 - COUNT(submit_time) / COUNT(1)) DESC
      ) AS ranking
    FROM
      tmp
    WHERE
      tag = 'SQL'
    GROUP BY
      uid
  ) s1
  JOIN (
    SELECT
      uid,
      DATE_FORMAT(start_time, '%Y%m') AS start_month,
      COUNT(1) AS total_cnt,
      COUNT(submit_time) AS complete_cnt,
      ROW_NUMBER() OVER(
        PARTITION BY uid
        ORDER BY
          DATE_FORMAT(start_time, '%Y%m') DESC
      ) AS ranking
    FROM
      tmp
    GROUP BY
      uid,
      DATE_FORMAT(start_time, '%Y%m')
  ) s2 ON s1.uid = s2.uid
WHERE
  s1.ranking <= 0.5
  AND s1.level IN (6, 7)
  AND s2.ranking <= 3
ORDER BY
  uid,
  start_month;
```

## 141 试卷完成数同比2020年的增长率及排名变化
```sql
WITH tmp AS (
  SELECT
    er.*,
    ei.tag
  FROM
    exam_record er
    LEFT JOIN examination_info ei ON er.exam_id = ei.exam_id
  WHERE
    er.submit_time IS NOT NULL
)
SELECT
  p1.tag,
  p1.exam_cnt AS exam_cnt_20,
  p2.exam_cnt AS exam_cnt_21,
  CONCAT(
    ROUND(((p2.exam_cnt - p1.exam_cnt) / p1.exam_cnt) * 100, 1),
    '%'
  ) AS growth_rate,
  p1.exam_cnt_rank AS exam_cnt_rank_20,
  p2.exam_cnt_rank AS exam_cnt_rank_21,
  CAST(p2.exam_cnt_rank AS signed) - CAST(p1.exam_cnt_rank AS SIGNED) AS rank_delta
FROM
  (
    SELECT
      tag,
      COUNT(1) AS exam_cnt,
      RANK() OVER(
        ORDER BY
          COUNT(1) DESC
      ) AS exam_cnt_rank
    FROM
      tmp
    WHERE
      YEAR(start_time) = '2020'
      AND DATE_FORMAT(start_time, '%Y%m') <= '202006'
    GROUP BY
      tag
  ) p1
  JOIN (
    SELECT
      tag,
      COUNT(1) AS exam_cnt,
      RANK() OVER(
        ORDER BY
          COUNT(1) DESC
      ) AS exam_cnt_rank
    FROM
      tmp
    WHERE
      YEAR(start_time) = '2021'
      AND DATE_FORMAT(start_time, '%Y%m') <= '202106'
    GROUP BY
      tag
  ) p2 ON p1.tag = p2.tag
ORDER BY
  growth_rate DESC,
  exam_cnt_rank_21 DESC;
```


## SQL142 对试卷得分做min-max归一化

```sql
SELECT  uid
       ,exam_id
       ,ROUND(AVG(IF(min_val != max_val,(score-min_val)*100/(max_val-min_val),score))) AS avg_new_score
FROM
(
	SELECT  i.exam_id
	       ,r.uid
	       ,r.score
	       ,MAX(r.score) OVER(PARTITION BY i.exam_id ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING) AS max_val
	       ,MIN(r.score) OVER(PARTITION BY i.exam_id ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING) AS min_val
	FROM examination_info i
	INNER JOIN exam_record r
	ON i.exam_id = r.exam_id
	WHERE i.difficulty = 'hard'
	AND score IS NOT NULL 
)p
GROUP BY  uid
         ,exam_id
ORDER BY exam_id
         ,avg_new_score DESC;

```
```sql
-- 更稳妥的做法,注意看窗口函数那里，写不写ROWS BETWEEN是一样的
WITH tmp AS (
  SELECT
    er.*,
    ei.difficulty
  FROM
    exam_record er
    LEFT JOIN examination_info ei ON er.exam_id = ei.exam_id
  WHERE
    difficulty = 'hard'
    AND submit_time IS NOT NULL
)
SELECT
  uid,
  exam_id,
  ROUND(AVG(IF(cnt <> 1, (score - mi) * 100 / (mx - mi), score))) AS avg_new_score
FROM
  (
    SELECT
      uid,
      score,
      exam_id,
      MAX(score) OVER(PARTITION BY exam_id) AS mx,
      MIN(score) OVER(PARTITION BY exam_id) AS mi,
      COUNT(1) OVER(PARTITION BY exam_id) AS cnt
    FROM
      tmp
  ) d
GROUP BY
  uid,
  exam_id
ORDER BY
  exam_id,
  avg_new_score DESC;
```


## SQL34 每份试卷每月作答数和截止当月的作答总数
```sql
SELECT  exam_id
       ,date_format(start_time,'%Y%m')                                                   AS start_month
       ,COUNT(*)                                                                         AS month_cnt
       ,SUM(COUNT(*)) OVER(PARTITION BY exam_id ORDER BY date_format(start_time,'%Y%m')) AS cum_exam_cnt
FROM exam_record
GROUP BY  exam_id
         ,date_format(start_time,'%Y%m')
ORDER BY exam_id
         ,start_month;
```

## SQL35 每月及截止当月的答题情况

```sql
SELECT  start_month
       ,COUNT(DISTINCT uid)                                             AS mau
       ,COUNT(IF(ranking = 1,uid,NULL))                                 AS month_add_uv
       ,MAX(COUNT(IF(ranking = 1,uid,NULL))) OVER(ORDER BY start_month) AS max_month_add_uv
       ,SUM(COUNT(IF(ranking = 1,uid,NULL))) OVER(ORDER BY start_month) AS cum_sum_uv
FROM
(
	SELECT  uid
	       ,date_format(start_time,'%Y%m')                          AS start_month
	       ,row_number() OVER(PARTITION BY uid ORDER BY start_time) AS ranking
	FROM exam_record
) s
GROUP BY  start_month;
```
##### Note
over() 里面可以什么都不写，这时候就是不分窗口也不排序，对所有数据直接进行操作。