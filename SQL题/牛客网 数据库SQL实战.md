# 牛客网 数据库SQL实战

## 1 查找最晚入职员工的所有信息

```sql
SELECT * FROM employees
WHERE hire_date=
(SELECT MAX(hire_date) FROM employees);
```

```sql
SELECT * FROM employees ORDER BY hire_date DESC LIMIT 1;
```

##### Note

1. 做SQL题，先有清晰完整思路，再去写代码
2. 注意优先级顺序,括号用来改变优先级，括号中的内容最先计算
3. 对于时间来说，越靠后越大，可以使用MAX，MIN
4. SQL中的聚合函数后面不一定要跟GROUP BY，也可以跟WHERE或者什么都不跟；同理GROUP BY前面也不一定要有聚合函数，例子见下面第8题

## 2 查找入职员工时间排名倒数第三的员工所有信息

```sql
SELECT * FROM employees ORDER BY hire_date DESC LIMIT 2,1;
```

##### Note

1. LIMIT 2,1等价于LIMIT 1 OFFSET 2，即从第二个（从0开始计算）元素开始（包括这个元素自身），一页最多一个

## 3 查找当前薪水详情以及部门编号dept_no

```sql
-- 使用连接查询
SELECT s.emp_no,s.salary,s.from_date,s.to_date,d.dept_no
FROM salaries s INNER JOIN dept_manager d
ON d.emp_no = s.emp_no
AND d.to_date = '9999-01-01' AND s.to_date = '9999-01-01';
```

```sql
-- 使用连接查询
SELECT s.emp_no,s.salary,s.from_date,s.to_date,d.dept_no
FROM salaries s INNER JOIN dept_manager d
ON d.emp_no = s.emp_no AND d.to_date = '9999-01-01' AND s.to_date = '9999-01-01';
```

```sql
-- 使用多表查询
SELECT s.emp_no,s.salary,s.from_date,s.to_date,d.dept_no
FROM salaries s,dept_manager d
WHERE d.emp_no = s.emp_no AND d.to_date = '9999-01-01' AND s.to_date = '9999-01-01';
```

##### Note

1. 使用联结，都是要先把表联结起来成为新表，才接着进行其他操作，所以WHERE,LIMIT要放在联结的后面

2. 这里必须要把s作为主表，且两个时间都要满足，才能通过，不知道为什么，可能是牛客网的文字游戏

## 4 查找所有已经分配部门的员工的last_name和first_name以及dept_no

```sql
SELECT e.last_name,e.first_name,d.dept_no
FROM employees e INNER JOIN dept_emp d
ON e.emp_no = d.emp_no;
```

## 5 查找所有员工的last_name和first_name以及对应部门编号dept_no

```sql
SELECT e.last_name,e.first_name,d.dept_no
FROM employees e LEFT OUTER JOIN dept_emp d
ON e.emp_no = d.emp_no;
```

##### Note

1. 如果一个员工没有分配部门，那么dept_emp这张表里就不会有他的记录
2. LEFT JOIN是LEFT OUTER JOIN的简写，保险起见还是用后者比较好

## 6 查找所有员工入职时候的薪水情况

```sql
SELECT e.emp_no,s.salary
FROM employees e,salaries s
WHERE e.emp_no = s.emp_no AND e.hire_date=s.from_date
ORDER BY e.emp_no DESC;
```

##### Note

对于e.emp_no这一列，列名就是emp_no,如果不加别名在客户机上看到的就是emp_no，加别名则显示别名。即客户机上看到的永远是别名/非完全限定列名。但是在代码中的时候，为了不产生歧义，必须使用完全限定列名。即可能产生歧义的列名，在SQL代码中一律使用完全限定列名

## 7 查找薪水涨幅超过15次的员工号emp_no以及其对应的涨幅次数t

```sql
SELECT a.* FROM
(SELECT emp_no,count(*) t FROM salaries GROUP BY emp_no) a
WHERE t>15
```

```sql
SELECT emp_no,count(*) t FROM salaries GROUP BY emp_no
HAVING t>15
```

##### Note

注意区分WHERE和HAVING：

1. https://www.cnblogs.com/ljf-Sky/p/9024683.html
2. WHERE是从FROM的表里选，HAVING 是从SELECT后的表里选

## 8 找出所有员工当前薪水salary情况

使用GROUP BY

```sql
SELECT salary FROM salaries WHERE to_date='9999-01-01' GROUP BY salary ORDER BY salary DESC;
```

使用DISTINCT

```sql
SELECT DISTINCT salary FROM salaries WHERE to_date = '9999-01-01' ORDER BY salary DESC;
```

##### Note

1. 对于distinct与group by的使用: 
   - 当对系统的性能高并数据量大时使用group by，大表一般用distinct效率不高，大数据量的时候都禁止用distinct
   - 当对系统的性能不高时使用数据量少时两者皆可 
   - 尽量使用group by，但也要会用DISTINCT
2. 一般只有在子查询和运算符优先级确定的时候才用括号，其他情况不用，比如上面如果给```DISTINCT salary```加括号就会报错
3. ORDER BY 后面跟的应该是SELECT和HAVING结束后的列名，即跟之前的运算无关，之前运算选择完数据了，它再根据某一列排序
4. DISTINCT可以用于SELECT后面，列名前面，也可以用于聚合函数中。作用域是它后面的所有列

## 9 获取所有部门当前manager的当前薪水情况，给出dept_no, emp_no以及salary，当前表示to_date='9999-01-01'

```sql
SELECT d.dept_no,d.emp_no,s.salary
FROM dept_manager d INNER JOIN salaries s
ON d.emp_no=s.emp_no AND d.to_date = '9999-01-01' AND S.to_date = '9999-01-01';
```

##### Note

牛客网上的题，凡是涉及到date的，一般都要两个表时间同时成立才行

## 10 获取所有非manager的员工emp_no

```sql
SELECT emp_no FROM employees
WHERE emp_no NOT IN (SELECT emp_no FROM dept_manager);
```

## 11 获取所有员工当前的manager

```sql
SELECT e.emp_no,m.emp_no manager_no
FROM dept_emp e INNER JOIN dept_manager m
ON e.dept_no = m.dept_no AND e.to_date='9999-01-01' AND m.to_date = '9999-01-01'
AND e.emp_no <> m.emp_no;
```

## 12 获取所有部门中当前员工薪水最高的相关信息

```sql
-- 正确答案1，非关联子查询
SELECT a.dept_no,b.emp_no,a.maxSalary
FROM
(SELECT d.dept_no,MAX(s.salary) AS maxSalary
FROM dept_emp d INNER JOIN salaries s
ON d.emp_no = s.emp_no AND d.to_date = "9999-01-01" AND s.to_date = "9999-01-01"
GROUP BY d.dept_no) a
LEFT JOIN
(SELECT d.dept_no,d.emp_no,s.salary
FROM dept_emp d INNER JOIN salaries s
ON d.emp_no = s.emp_no AND d.to_date = "9999-01-01" AND s.to_date = "9999-01-01") b
ON a.dept_no = b.dept_no AND a.maxSalary = b.salary
ORDER BY a.dept_no;
```

```sql
-- 正确答案2，关联子查询
SELECT  de.dept_no
       ,de.emp_no
       ,s.salary
FROM dept_emp de
INNER JOIN salaries s
ON de.emp_no = s.emp_no AND de.to_date = '9999-01-01' AND s.to_date = '9999-01-01'
WHERE s.salary = ( 
SELECT  MAX(s2.salary)
FROM dept_emp de2
INNER JOIN salaries s2
ON de2.emp_no = s2.emp_no AND de2.to_date = '9999-01-01' AND s2.to_date = '9999-01-01'
WHERE de2.dept_no = de.dept_no 
GROUP BY  de2.dept_no)
ORDER BY de.dept_no
```

```sql
-- 正确答案3，窗口函数
SELECT  a.dept_no
       ,a.emp_no
       ,a.salary AS maxSalary
FROM 
(
	SELECT  d.dept_no
	       ,d.emp_no
	       ,s.salary
	       ,ROW_NUMBER() OVER(PARTITION BY d.dept_no ORDER BY s.salary DESC) AS ranking
	FROM dept_emp d
	INNER JOIN salaries s
	ON d.emp_no = s.emp_no AND d.to_date = "9999-01-01" AND s.to_date = "9999-01-01"
) a
WHERE a.ranking = 1
ORDER BY a.dept_no;
```

##### Note

1. 聚合函数聚合的是筛选之后的数据，或者说SELECT选择的就是筛选之后的数据中的几列

2. 如果是COUNT(),SUM(),AVG()那么聚合函数之前只能跟聚合后值唯一的列，但如果是MAX(),MIN()那么聚合函数之前也可以跟其他列，结果就是最大/最小对应的那一行

4. SQL语句的书写顺序是SELECT...FROM...[JOIN...ON...]WHERE...GROUP BY...HAVING...ORDER BY...LIMIT

   执行顺序是:

   - 先把表拼在一起 FROM...[JOIN...ON...]
   
   - 再筛选 WHERE...
   
   - 再聚合 GROUP BY...
   
   - 对分组聚合结果进行筛选HAVING...
   
   - 再选列 SELECT... 
   
   - 排序 ORDER BY...
   
   - 分页输出 LIMIT
   
     每一步都是在上一步执行后形成的表中进行操作。其中需要注意的是，HAVING 筛选的时候本来后面只能跟分组列和聚合函数列，但后来兼容了，可以用SELECT里面的列名/别名（但很多书中都不用，因为不规范），比如SELECT里面给聚合函数列起了别名，HAVING就可以拿来用。
   
4. GROUP BY,ORDER BY虽然是在上一步的表的基础进行操作的，但后面不一定要跟上一步表中的列，也可以跟对上一步表中的列的处理，可以理解为对上一步表中的列进行处理产生新列，然后对新列进行GROUP或者ORDER，。GROUP BY 后面不一定要跟表中的列，也可以对表中列的处理（比如IF/SUBSTRING之类的函数或CASE WHEN END新建的列）。ORDER BY 后面不光可以跟SELECT后面的列，还可以跟SELECT后面的列的处理（比如SUBSTRING函数啥的）

5. 在正确答案1当中，子查询先于包含语句执行，相同优先级的子查询从左到右执行，因此先执行第一个子查询接着第二个，在每一个子查询执行时都不会产生歧义，因此两个子查询当中可以用相同的别名。

6. 在正确答案3当中，如果别名变成rank/row_number/dense_rank，会报错，推测可能是占用保留字导致。

```sql
-- 典型错误答案
SELECT d.dept_no,s.emp_no,MAX(s.salary) salary FROM dept_emp d INNER JOIN salaries s 
ON d.emp_no = s.emp_no AND d.to_date='9999-01-01' AND s.to_date='9999-01-01'
GROUP BY d.dept_no;
```

##### Note

1. 使用GROUP BY子句时，select中只能有聚合键、聚合函数、常数。，如果有其他列，会默认显式为本组第一行记录对应的值，如果没有GROUP BY 直接聚合，会默认显式这个表中第一行记录对应的值。从这个角度讲，如果SELECT后面跟非聚合键，但是是聚合之后在组中值唯一的列，最后显示的结果也是正确的，因为在组中值唯一，因此取组中第一行也没问题。
2. emp_no并不符合这个要求，所以其值是在这个分组中随机选一个返回的，或者默认取分组中的第一个。所以有时候运行可以得到max(salary)对应的emp_no，有时得不到，全看运气，但就算恰好蒙对了，也不代表这个语法是正确的。

## 13 从titles表获取按照title进行分组

```sql
SELECT title,COUNT(*) t FROM titles GROUP BY title HAVING t>=2;
```

##### Note

子句顺序https://blog.csdn.net/qq_23864697/article/details/80765371

## 14 从titles表获取按照title进行分组，注意对于重复的emp_no进行忽略。

```sql
SELECT title,COUNT(DISTINCT emp_no) t FROM titles GROUP BY title HAVING t>=2;
```

## 15 查找employees表

```sql
SELECT * FROM employees 
WHERE emp_no % 2 = 1 AND last_name <> 'Mary'
ORDER BY hire_date DESC;
```

##### Note

感觉sql支持的算术运算符和python差不多啊

## 16 统计出当前各个title类型对应的员工当前薪水对应的平均工资

```sql
SELECT title,avg(salary) avg FROM titles t INNER JOIN salaries s
ON t.emp_no = s.emp_no AND t.to_date = '9999-01-01' AND s.to_date = '9999-01-01'
GROUP BY title;
```

## 17 获取当前薪水第二多的员工的emp_no以及其对应的薪水salary

```sql
-- 典型错误答案，见12题解释
SELECT emp_no,MAX(salary) salary FROM salaries
WHERE salary<(SELECT MAX(salary) FROM salaries WHERE to_date='9999-01-01') 
AND to_date='9999-01-01';
```

下面是参考别人的更具有普适性的做法，可以应对薪水第3多的，第4多的等等

```sql
SELECT emp_no,salary FROM salaries WHERE to_date='9999-01-01' ORDER BY salary DESC LIMIT 1,1;
```

```sql
SELECT emp_no,salary FROM salaries WHERE to_date = '9999-01-01' AND salary = 
(SELECT DISTINCT salary FROM salaries WHERE to_date = '9999-01-01' ORDER BY salary DESC LIMIT 1,1);
```

## 18 获取当前薪水第二多的员工的emp_no以及其对应的薪水salary，不准使用order by

```sql
-- 不使用窗口函数，因为不准使用ORDER BY
SELECT  e.emp_no
       ,s.salary
       ,e.last_name
       ,e.first_name
FROM employees e
INNER JOIN salaries s
ON e.emp_no = s.emp_no
WHERE s.salary = ( 
SELECT  MAX(salary) salary
FROM salaries
WHERE salary<( 
SELECT  MAX(salary)
FROM salaries
WHERE to_date='9999-01-01') AND to_date='9999-01-01');  
```

```sql
-- 典型错误答案，见12题解释
SELECT e.emp_no,MAX(s.salary) salary,e.last_name,e.first_name 
FROM employees e INNER JOIN salaries s
ON e.emp_no=s.emp_no AND s.to_date = '9999-01-01'
WHERE s.salary<(SELECT MAX(s.salary) FROM salaries s WHERE s.to_date='9999-01-01');
```

## 19  查找所有员工的last_name和first_name以及对应的dept_name

```sql
SELECT e.last_name,e.first_name,dp.dept_name 
FROM employees e LEFT OUTER JOIN dept_emp d ON e.emp_no=d.emp_no
LEFT OUTER JOIN departments dp on d.dept_no = dp.dept_no;
```

##### Note

第一次左联结的结果中，dept_no这一列有很多NULL

第二次也要使用左联结，departments表对于NULL值无法匹配，所以会保留一行，右表值全部为NULL，结果没问题

ON和WHERE一样，只有当返回TRUE的时候才代表匹配成功。而对于=运算符来说，左右两边任意一个为NULL，就会返回NULL，匹配失败，如果第二个联结使用INNER JOIN，那么这一行就会被过滤掉。

## 20 查找员工编号emp_no为10001其自入职以来的薪水salary涨幅值growth

```sql
SELECT MAX(salary)-MIN(salary) growth FROM salaries WHERE emp_no=10001;
```

上面的做法当最后一次为降薪的时候不成立，的更严谨的做法如下：

```sql
SELECT ( 
(SELECT salary FROM salaries WHERE emp_no = 10001 ORDER BY from_date DESC LIMIT 1) -
(SELECT salary FROM salaries WHERE emp_no = 10001 ORDER BY from_date ASC LIMIT 1)
) AS growth;
```

## 21 查找所有员工自入职以来的薪水涨幅情况

```sql
SELECT start.emp_no,current.salary - start.salary growth FROM
(SELECT e.emp_no,s.salary FROM employees e INNER JOIN salaries s ON e.emp_no = s.emp_no 
AND e.hire_date = s.from_date) start INNER JOIN
(SELECT e.emp_no,s.salary FROM employees e INNER JOIN salaries s ON e.emp_no = s.emp_no 
AND s.to_date = '9999-01-01') current
ON start.emp_no = current.emp_no ORDER BY growth;
```

---

##### Note

1. 见Mysql必知必会第10章，计算字段
2. 给算术计算字段加别名时，算术运算不用加括号
2. 不要害怕复杂，当思考后觉得简单的无法完成题目要求就大胆上复杂的，不要耽误时间
4. 因为两个子查询独立进行不会产生歧义，因此可以在子查询内部用相同的别名

---

## 22 统计各个部门的工资记录数

```sql
SELECT d.dept_no,d.dept_name,count(m.salary) sum FROM departments d INNER JOIN
(SELECT de.dept_no,de.emp_no,s.salary FROM dept_emp de INNER JOIN salaries s ON de.emp_no = s.emp_no) m 
ON d.dept_no = m.dept_no GROUP BY d.dept_no;
```

```sql
SELECT dem.dept_no,dep.dept_name,COUNT(*) sum
FROM dept_emp dem INNER JOIN salaries s ON dem.emp_no = s.emp_no INNER JOIN 
departments dep ON dem.dept_no = dep.dept_no GROUP BY dem.dept_no;
```

---

##### Note

1. 可以将子查询的结果作为连接表，也可以对三个表进行连接，见SQL学习指南第5章 多表查询
2. 将非分组列和非聚合列放在SELECT中会在组中随机选一个赋值（一般是第一个），如果这个列在分组后值唯一，就没问题。

---

## 23 对所有员工的薪水按照salary进行按照1-N的排名

```sql
SELECT emp_no, salaries, DENSE_RANK() OVER(ORDER BY salary DESC) AS rank
WHERE to_date = '9999-01-01' ORDER BY salary DESC, emp_no ASC;
```

1. 窗口函数的使用：

   https://www.cnblogs.com/DataArt/p/9961676.html

   注意，窗口函数的分组是在where之后的，因为它还是在SELECT子句当中的

2. 注意，序号的窗口函数是按照排序进行排序号的

3. 但是牛客网并不支持窗口函数，所以上述代码无法通过，要使用以下语句

```sql
SELECT s1.emp_no, s1.salary, COUNT(DISTINCT s2.salary) AS rank
FROM salaries AS s1, salaries AS s2
WHERE s1.to_date = '9999-01-01'  AND s2.to_date = '9999-01-01' AND s1.salary <= s2.salary
GROUP BY s1.emp_no
ORDER BY s1.salary DESC, s1.emp_no ASC;
```

```sql
-- 一个容易犯的错误
SELECT  emp_no
       ,salary
       ,DENSE_RANK() OVER(ORDER BY salary DESC,emp_no) AS t_rank
FROM salaries s
WHERE to_date = '9999-01-01'; 
```

这里出错的原因是，答案想要的结果是如果两个员工salary相等，排名就相等，但是emp_no小的排在前面，但如果这样写sql，salary相同的员工排名并不相等，因为还有emp_no排名，所以在排序窗口函数看来并不是并列的了。

## 24 获取所有非manager员工当前的薪水情况

```sql
SELECT d.dept_no,d.emp_no,s.salary FROM dept_emp d INNER JOIN salaries s ON d.emp_no = s.emp_no
WHERE d.to_date = '9999-01-01' AND s.to_date = '9999-01-01' AND d.emp_no NOT IN 
(SELECT emp_no FROM dept_manager WHERE to_date = '9999-01-01');
```

## 25 获取员工其当前的薪水比其manager当前薪水还高的相关信息

```sql
SELECT l1.emp_no,l2.emp_no manager_no,l1.salary emp_salary,l2.salary manager_salary FROM
(SELECT d.emp_no,d.dept_no,s.salary FROM dept_emp d INNER JOIN salaries s
 ON d.emp_no  = s.emp_no AND d.to_date ='9999-01-01' AND s.to_date = '9999-01-01') l1,
(SELECT d.emp_no,d.dept_no,s.salary FROM dept_manager d INNER JOIN salaries s 
 ON d.emp_no  = s.emp_no AND d.to_date ='9999-01-01' AND s.to_date = '9999-01-01') l2
WHERE l1.dept_no = l2.dept_no AND l1.salary>l2.salary;
```

## 26 汇总各个部门当前员工的title类型的分配数目

```sql
SELECT d.dept_no,d.dept_name,t.title,COUNT(t.title) count
FROM departments d INNER JOIN dept_emp p  ON d.dept_no = p.dept_no AND p.to_date = '9999-01-01'
INNER JOIN titles t ON p.emp_no = t.emp_no AND t.to_date = '9999-01-01'
GROUP BY d.dept_no,t.title;
```

---

##### Note

嵌套分组聚合见Mysql必知必会第13章分组数据

嵌套分组后使用聚合函数会在最后一次分组的结果上进行聚合

---

## 27 给出每个员工每年薪水涨幅超过5000的员工编号emp_no

```sql
SELECT s2.emp_no, s2.from_date, (s2.salary - s1.salary) AS salary_growth
 FROM salaries AS s1, salaries AS s2
 WHERE s1.emp_no = s2.emp_no
 AND s2.salary - s1.salary > 5000
 AND strftime('%Y',s2.to_date) - strftime('%Y',s1.to_date) = 1
 ORDER BY salary_growth DESC;
```

以下会报错：

```sql
SELECT s2.emp_no,s2.from_date,s2.salary-s1.salary salary_growth
FROM salaries s1 INNER JOIN salaries s2 ON s1.emp_no = s2.emp_no
AND strftime('%Y',s2.to_date)-strftime('%Y',s1.to_date)=1 
HAVING salary_growth>5000
ORDER BY salary_growth DESC;
```

---

##### Note 

stack overflow上是这样解释的：SQL标准要求HAVING必须仅引用GROUP BY子句中的列或聚合函数列(可以是跟在SELECT后面的也可以不是)。但是，MySQL支持对此行为的扩展，并允许HAVING引用SELECT关键字后面的列以及外部子查询中的列。

sqlite应该是不支持这种扩展所以上面第二段代码不对

---

## 28 查找描述信息中包括robot的电影对应的分类名称以及电影数目，而且还需要该分类对应电影数量>=5

```sql
SELECT ls3.name,COUNT(ls1.film_id) num
FROM film ls1 INNER JOIN film_category ls2 ON ls1.film_id = ls2.film_id
INNER JOIN category ls3 ON ls2.category_id = ls3.category_id  AND ls2.category_id IN 
(SELECT ls5.category_id FROM film ls4 INNER JOIN film_category ls5 ON ls4.film_id = ls5.film_id
GROUP BY ls5.category_id HAVING COUNT(ls4.film_id)>=5)
WHERE ls1.description LIKE '%robot%' GROUP BY ls3.name;
```

```sql
SELECT c.name,COUNT(f.film_id) FROM film f INNER JOIN film_category fc 
ON f.film_id = fc.film_id AND f.description LIKE '%robot%' 
INNER JOIN category c ON fc.category_id = c.category_id AND fc.category_id IN
(SELECT fc2.category_id FROM film f2 INNER JOIN film_category fc2 ON f2.film_id = fc2.film_id 
 GROUP BY fc2.category_id HAVING COUNT(f2.film_id)>=5)
GROUP BY c.category_id;
```

##### Note

1. 题目中电影数目>=5 是这类电影的所有数目，并不是包含了robot的这类电影的数目，这里很坑，属于题目表述不当
2. 虽然说使用COUNT/SUM等聚合函数时SELECT后面跟的不一定是分组列，只要是分组后值唯一的列就可以，但是能用分组列的时候还是尽量用分组列，确保不会出错

## 29 使用join查询方式找出没有分类的电影id以及名称

```sql
SELECT f.film_id,f.title FROM film f LEFT JOIN film_category fc ON f.film_id = fc.film_id 
WHERE fc.category_id IS NULL;
```

## 30 使用子查询的方式找出属于Action分类的所有电影对应的title,description

```sql
SELECT f.title,f.description FROM film f WHERE f.film_id IN
(SELECT fc.film_id FROM film_category fc INNER JOIN category c ON 
 fc.category_id = c.category_id AND c.name = 'Action');
```

## 31 获取select * from employees对应的执行计划

```sql
EXPLAIN SELECT * FROM employees;
```

##### Note

1. 通过 explain 命令获取 select 语句的执行计划：
   - https://www.cnblogs.com/butterfly100/archive/2018/01/15/8287569.html
   - https://blog.csdn.net/jianghao_ava/article/details/83187414

2. 在SELECT语句前加EXPLAIN关键字，返回SELECT语句的执行计划，而不会执行SELECT语句（如果 from 中包含子查询，仍会执行该子查询，将结果放入临时表中）。执行计划包括访问表的顺序，查询的操作类型，哪些索引可以使用，哪些索引被实际使用 等等。
3. id 关键字按照SELECT的出现顺序排列，大小按照执行顺序赋值，id相同，执行顺序从上往下；id不同，id值越大，优先级越高，越先执行

## 32 将employees表的所有员工的last_name和first_name拼接起来作为Name

在本题的SQLite中：

```sql
SELECT last_name||" "||first_name Name FROM employees;
```

MySQL支持CONCAT方法:

```sql
select CONCAT(CONCAT(last_name," "),first_name) as Name  from employees
或者
select CONCAT(last_name," "，first_name) as Name  from employees
```

##### Note

1. SQL字符串相关函数见SQL学习指南第7章第1节，Mysql必知必会第11章
2. 需要掌握的字符串处理函数有UPPER,LOWER,LEFT,RIGHT,LTRIM,RTRIM,INSERT,REPLACE,SUBSTRING,LENGTH,POSITION,LOCATE,CONCAT,GROUP_CONCAT,

## 33 创建一个actor表，包含如下列信息

```sql
CREATE TABLE actor(actor_id smallint(5) NOT NULL,
                  first_name varchar(45) NOT NULL,
                  last_name varchar(45) NOT NULL,
                  last_update timestamp NOT NULL DEFAULT(datetime('now','localtime')),
                  PRIMARY KEY(actor_id));
```

##### Note

1. 见Mysql必知必会第21章 创建和操纵表，以及廖的Mysql教程
2. 牛客使用sqlite对部分大小写敏感，刚开始字段类型和datetime函数用大写一直无法通过，真的坑
3. DEFAULT后面按理说是不用加括号的，不知道为什么这里不加括号无法通过
4. Mysql的获取当前时间的函数与牛客网使用的sqlite不同
5. SQL日期时间相关函数见SQL学习指南第7章第3节,Mysql必知必会第11章
6. 需要掌握的事件处理函数有DATE,TIME,YEAR,MONTH,STR_TO_DATE,CURRENT_DATE/TIME/TIMESTAMP,NOW,DATEDIFF

## 34 批量插入数据

```sql
INSERT INTO actor VALUES(1,'PENELOPE','GUINESS','2006-02-15 12:34:33'),
(2,'NICK','WAHLBERG','2006-02-15 12:34:33');
```

##### Note

虽然时间数据在数据库里是特定时间类型，但是我们在插入/过滤的时候只要使用规范格式的字符串即可，数据库会自行识别转换为时间类型

## 35 批量插入数据，不使用replace操作

```sql
INSERT OR IGNORE INTO actor VALUES (3,'ED','CHASE','2006-02-15 12:34:33');
```

##### Note

1. 因为是在SQLite中，所以是INSERT OR IGNORE INTO，如果在MySQL中，需要把OR去掉
2. REPLACE INTO,ON DUPLICATE KEY等的用法见如下链接：
   - https://www.cnblogs.com/flyingeagle/articles/9825949.html
   - https://www.cnblogs.com/sweet521/p/5730804.html

3. 在插入数据时，无论是哪个语句，所检查的都是PRIMARY KEY或UNIQUE INDEX，也就是这两个中任意一个重复就不能直接插入

## 36 创建一个actor_name表

```sql
CREATE TABLE actor_name AS SELECT first_name,last_name FROM actor;
```

##### Note

因为是在SQLite中，所以要有AS，如果在MySQL中，AS可以有可以没有

## 37 对first_name创建唯一索引uniq_idx_firstname

在本题的SQLite中：

```sql
CREATE UNIQUE INDEX uniq_idx_firstname ON actor(first_name);
CREATE INDEX idx_lastname ON actor(last_name);
```

在MySQL中上面的代码也正确，也可以使用如下代码：

```sql
ALTER TABLE actor ADD UNIQUE uniq_idx_firstname(first_name);
ALTER TABLE actor ADD INDEX idx_lastname(last_name);
```

##### Note

索引和约束相关知识见SQL学习指南第13章 索引和约束

## 38 针对actor表创建视图actor_name_view

```sql
CREATE VIEW actor_name_view AS SELECT  first_name first_name_v,last_name last_name_v
FROM actor;
```

##### Note

视图相关知识见SQL学习指南第14章 视图  Mysql必知必会第22章 使用视图

## 39 针对上面的salaries表emp_no字段创建索引idx_emp_no

```sql
SELECT * FROM salaries INDEXED BY idx_emp_no WHERE emp_no = 10005;
```

##### Note

题目这里已经默认给创建好索引了，如果再创建会出错

通常来说，优化器会自动选择合适的索引，如果要强制使用某个索引的话如下所示

SQLite中，使用 INDEXED BY 语句进行强制索引查询，可参考：

http://www.runoob.com/sqlite/sqlite-indexed-by.html

MySQL中，使用 FORCE INDEX 语句进行强制索引查询，可参考：

http://blog.sina.com.cn/s/blog_6004d60d0102xr8y.html

## 40 在last_update后面新增加一列名字为create_date

```sql
ALTER TABLE actor ADD create_date datetime NOT NULL DEFAULT '0000-00-00 00:00:00';
```

##### Note

1. 这里的datetime纯粹为了跟标准答案一致才用的小写
2. DEFAULT 要放在NOT NULL 之后

## 41 构造一个触发器audit_log

```sql
CREATE TRIGGER audit_log AFTER INSERT ON employees_test FOR EACH ROW 
BEGIN
INSERT INTO audit VALUES(NEW.ID,NEW.NAME);
END;
```

在Mysql中上述代码无法执行，应该使用如下代码：

```sql
-- 第一种方法
CREATE TRIGGER audit_log AFTER INSERT ON employees_test FOR EACH ROW 
INSERT INTO audit VALUES(NEW.ID,NEW.NAME);
-- 第二种方法
DELIMITER //
CREATE TRIGGER audit_log AFTER INSERT ON employees_test FOR EACH ROW 
BEGIN
INSERT INTO audit VALUES(NEW.ID,NEW.NAME);
END//
DELIMITER ;
```

##### Note

1. 触发器知识见Mysql必知必会 第25章 使用触发器
2. 补充知识见如下链接：
   - https://www.cnblogs.com/geaozhang/p/6819648.html
   - https://blog.csdn.net/zhangai8351205/article/details/84144361
3. Mysql中的变量：https://www.cnblogs.com/Brambling/p/9259375.html
4. 通常begin-end用于定义一组语句块，其中可以可以包含多条SQL语句，在各大数据库中的客户端工具中可直接调用，但在mysql中不可（需要更改结束符）。见如下链接：
   - https://www.cnblogs.com/oDoraemon/p/5881885.html
   - https://blog.csdn.net/yuxin6866/article/details/52722913
5. 在sqlite中，触发器中必须使用BEGIN-END语句块，但MYSQL中不必须，如果非要使用，需要先更改结束符。
6. FOR EACH ROW 是对于每一个相应变动行执行操作，即对于第一行执行下面的操作，对于第二行执行下面的操作，依次下去直到最后

## 42 删除emp_no重复的记录，只保留最小的id对应的记录。

```sql
DELETE FROM titles_test WHERE id NOT IN 
(SELECT MIN(id) FROM titles_test GROUP BY emp_no);
```

## 43 将所有to_date为9999-01-01的全部更新为NULL

```sql
UPDATE titles_test SET to_date = NULL,from_date = '2001-01-01' WHERE to_date = '9999-01-01';
```

##### Note

1. update 并非要全列更新，哪一列更新写哪一列，中间用逗号连接。
2. 空值的赋值也要用=，毕竟也没有其他赋值符号
3. INSERT INTO VALUES ON DUPLICATE UPDATE后面不用加SET直接什么=什么,什么=什么,...

## 44 将id=5以及emp_no=10001的行数据替换成id=5以及emp_no=10005

```sql
REPLACE INTO titles_test SELECT 5,10005,title,from_date,to_date FROM titles_test WHERE id = 5;
```

##### Note

1. 如果用SELECT语句填充值的话就不需要VALUES
2. REPLACE INTO 的用法见：https://www.cnblogs.com/zxf100/p/10882138.html
3. REPLACE INTO判断的是PRIMARY或UNIQUE，两者只要有一个重复就执行删除再插入操作，在插入的记录在之前所有记录的最后。由于相当于插入新的行，所以REPLACE INTO后面各个列的值都要齐全（考虑自增和默认值），要类似INSERT INTO 一样。
4. 集合操作这里要每一列都相同才算重复行，REPLACE INTO 等操作那里只要主键列和唯一约束列相同就算重复记录。其实想想也很容易理解

## 45 将titles_test表名修改为titles_2017

在本题的SQLite中：

```sql
ALTER TABLE titles_test RENAME TO titles_2017;
```

在MySQL中上面的代码也正确，但也可以使用如下代码：

```sql
RENAME TABLE titles_test TO titles_2017;
```

##### Note

1. 重命名/添加约束只记住使用ALTER 的就可以（可以认为是修改表结构）

2. 创建索引/视图/触发器只记住使用CREATE的就可以（可以认为是创建新东西）

## 46 在audit表上创建外键约束，其emp_no对应employees_test表的主键id

在本题的SQLite中，只能删除表再创建表

```sql
DROP TABLE audit;
CREATE TABLE audit(
    EMP_no INT NOT NULL,
    create_date datetime NOT NULL,
    FOREIGN KEY(EMP_no) REFERENCES employees_test(ID));
```

在MySQL中：

```sql
ALTER TABLE audit ADD CONSTRAINT fk_EMP_NO FOREIGN KEY(EMP_no) REFERENCES employees_test(ID);
```

##### Note

注意references 后面没有to

## 47 如何获取emp_v和employees有相同的数据no

```sql
SELECT * FROM employees INTERSECT SELECT * FROM emp_v;
```

##### Note

SQL中的集合操作见SQL学习指南第6章 使用集合，Mysql必知必会第17章 组合查询

## 48 将所有获取奖金的员工当前的薪水增加10%

```sql
UPDATE salaries SET salary = 1.1*salary WHERE to_date = '9999-01-01' AND emp_no IN(SELECT DISTINCT emp_no FROM emp_bonus);
```

##### Note

在MySQL中，赋值和比较运算符中的等于都是=

## 49 针对库中的所有表生成select count(*)对应的SQL语句

sqlite写法(牛客网通过)： 

```sql
select "select count(*) from "||name||";" as cnts from sqlite_master where type='table';
```

https://blog.csdn.net/xingfeng0501/article/details/7804378

 mysql写法(牛客网不通过，但是我在自己的mysql上运行通过)： 

```sql
select concat("select count(*) from ",table_name,";") as cnts from (select table_name from information_schema.tables) as new;
```

在MySQL中基本认为schema就是数据库，information_schema.tables保存了所有数据库中的所有数据表，其中，table_schema字段代表所在数据库，table_name字段代表表的名字。

## 50 将employees表中的所有员工的last_name和first_name通过(')连接起来。

```sql
SELECT last_name||"'"||first_name FROM employees;
```

## 51 查找字符串'10,A,B' 中逗号','出现的次数cnt

```sql
SELECT (length("10,A,B")-length(replace("10,A,B",",","")))/length(",") AS cnt;
```

##### Note

1. SQL字符串相关函数见SQL学习指南第7章第1节
2. SQL当中的replace函数是替换值，Excel中的replace函数是替换位置，类似于Mysql当中的INSERT ;  SQL当中的长度用LENGTH，Excel和Python当中的是len

## 52 获取Employees中的first_name

```sql
SELECT first_name FROM employees ORDER BY SUBSTR(first_name,LENGTH(first_name)-1,2);
```

或者

```sql
SELECT first_name FROM employees ORDER BY SUBSTR(first_name,-2,2);
```

##### Note

1. SQL字符串相关函数见SQL学习指南第7章第1节

2. 如果是Mysql应该改为substring函数，sqlite substr 函数等同于Mysql中的substring函数

   https://blog.csdn.net/l358366885/article/details/79973430

   作用和参数类似excel当中的mid函数

3. Python无论是表还是序列都是从0开始的，Excel无论是表还是序列都是从1开始的，当前时间是0，SQL的表索引是从0开始的，序列是从1开始的。负数索引这三个都一样

4. ORDER BY 后面不光可以跟SELECT后面的列，还可以跟SELECT后面的列的处理

## 53 按照dept_no进行汇总

在本题的SQLite中

```sql
SELECT dept_no,GROUP_CONCAT(emp_no,",") FROM dept_emp GROUP BY dept_no;
```

https://blog.csdn.net/langzxz/article/details/16807859

在MySQL中

```sql
SELECT dept_no,GROUP_CONCAT(emp_no SEPARATOR ",") FROM dept_emp GROUP BY dept_no;
```

https://blog.csdn.net/u012620150/article/details/81945004

MySQL中的这个GROUP_CONCAT函数，参数之间没有逗号

## 54 查找排除当前最大、最小salary之后的员工的平均工资avg_salary

```sql
SELECT AVG(salary) avg_salary FROM salaries WHERE to_date = '9999-01-01' AND salary BETWEEN
(SELECT salary FROM salaries ORDER BY salary LIMIT 1,1) 
AND (SELECT salary FROM salaries ORDER BY salary DESC LIMIT 1,1);
```

这里应该是默认了表中最大值最小值都是唯一的

## 55 分页查询employees表，每5行一页，返回第2页的数据

```sql
SELECT * FROM employees LIMIT (2-1)*5,5;
```

##### Note

从哪个位置开始，进行多少项，无论是函数(比如SUBSTRING,INSERT)还是分页查询，都是包含最初的位置的

## 56 获取所有员工的emp_no

```sql
SELECT e.emp_no,e.dept_no,b.btype,b.recevied 
FROM dept_emp e LEFT JOIN emp_bonus b ON e.emp_no = b.emp_no;
```

OUTER可以省略

## 57 使用含有关键字exists查找未分配具体部门的员工的所有信息。

```sql
SELECT * FROM employees WHERE NOT EXISTS(SELECT * FROM dept_emp 
                                         WHERE dept_emp.emp_no = employees.emp_no);
```

##### Note

SQL中EXISTS语句的使用：

见 SQL学习指南第9章 子查询，以及以下链接

https://blog.csdn.net/weixin_43901882/article/details/89467490

https://www.runoob.com/sql/sql-exists.html

https://www.cnblogs.com/xuanhai/p/5810918.html

## 58 获取employees中的行数据，且这些行也存在于emp_v中

```sql
SELECT * FROM employees where emp_no>10005;
```

什么智障题

## 59 获取有奖金的员工相关信息。

```sql
SELECT e.emp_no,e.first_name,e.last_name,b.btype,s.salary,
CASE b.btype WHEN 1 THEN 0.1*salary WHEN 2 THEN 0.2*salary ELSE 0.3*salary END bonus
FROM employees e INNER JOIN emp_bonus b ON e.emp_no = b.emp_no
INNER JOIN salaries s ON e.emp_no = s.emp_no AND s.to_date = '9999-01-01';
```

##### Note

1. SQL中CASE语句的使用：见 SQL学习指南第11章  条件逻辑
2. CASE语句后面一定要有END，END后面不加结束符，WHEN之间不用加逗号，一般会在END后面加别名，触发器在sqlite中一定要有BEGIN,END而且不用改结束符，在Mysql中可以没有BEGIN,END，如果要有的话，需要在语句最前面更改结束符DELIMITER，在语句最后面改回来

## 60 统计salary的累计和running_total

```sql
SELECT s1.emp_no,s1.salary,
(SELECT SUM(s2.salary) FROM salaries s2 WHERE s2.emp_no<=s1.emp_no AND s2.to_date = '9999-01-01')
running_total FROM salaries s1 WHERE s1.to_date = '9999-01-01';
```

##### Note

1. 题干叙述有误，running_total应该是所有小于等于的累加
2. 本题考查关联子查询
3. 关联子查询的执行顺序是对于上一步结果表的每一个候选行执行一次，如果出现在WHERE中就是对于FROM后面的表中的每一行查询一次，如果出现在SELECT中，就是对于筛选后的表每行执行一次。

## 61 对于employees表中，给出奇数行的first_name

```sql
SELECT e1.first_name FROM employees e1 WHERE (SELECT COUNT(*) FROM employees e2 
                                              WHERE e1.first_name>=e2.first_name)%2 = 1;
```

##### Note

1. 题干叙述有误,正确的题目应该是对于employees表，在对first_name进行排名后，选出奇数排名对应的first_name
2. 在Mysql中，上面代码也正确，但也可以使用下面的MOD(number1,number2)，求第一个数除以第二个数的余数

```sql
SELECT e1.first_name FROM employees e1 WHERE 
MOD((SELECT COUNT(*) FROM employees e2 WHERE e1.first_name>=e2.first_name),2) =1;
```

